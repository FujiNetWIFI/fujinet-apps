/**
 * uptodate.c
 *
 * Connect to fujinet.online to determine the latest official
 * firmware, and ask the #FujiNet what version you're running,
 * and tell you whether you're up-to-date!
 *
 * TODO:
 *  * Pausing/paginate (rather than just scrolling things off the top
 *    of the screen; you can use Ctrl+1 in the meantime)
 *
 * Bill Kendrick <bill@newbreedsoftware.com>
 *
 * License: v. 3, see LICENSE.md for details
 *
 * 2022-06-01 - 2022-06-01
 */

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sio.h"
#include "nsio.h"

/* https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24E8-Get-Adapter-Config */

// Response to SIO_FUJICMD_GET_ADAPTERCONFIG
typedef struct
{
    char ssid[32];
    char hostname[64];
    unsigned char localIP[4];
    unsigned char gateway[4];
    unsigned char netmask[4];
    unsigned char dnsIP[4];
    unsigned char macAddress[6];
    unsigned char bssid[6];
    char fn_version[15];
} adapterConfig_t;

adapterConfig_t adapterConfig;

void fuji_sio_read_adapter_config()
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xE8; /* Get Adapter Config ($E8) */
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = &adapterConfig;
  OS.dcb.dtimlo = 0x0f;
  OS.dcb.dbyt = sizeof(adapterConfig);
  OS.dcb.daux = 0;
  siov();
}

const char *url = "N:https://fujinet.online/firmware/releases_atari/releases.json";

/*
{
  "releases":
  [
    {
    "version": "0.5.b28073ee",
    "version_date": "2022-05-31 02:48:41",
    "build_date": "2022-05-31 02:48:41",
    "description": "Fix JSON bug, replace MIDIMaze mode with UDPStream mode",
    "git_commit": "b28073ee",
    "url": "https://fujinet.online/firmware/releases_atari/fujinet-ATARI-0.5.b28073ee.zip",
    "sha256": "c5599c9260c95a6bcfeda94d71a2f5fd63fc6d5971bcdc4a4b98092bdfba9b7c"
    },
    ...
  ],
}
*/

void inf_loop(void) {
  do { } while(1);
}

void abort(void) {
  printf("ABORTING\n");
  inf_loop();
}

void success_or_fail(unsigned char err) {
  if (err != 1 /* SUCCESS */) {
    printf("Error = %d\n", err);
    abort();
  }
}

#define NUM_ELEMENTS 3
const char * elements[NUM_ELEMENTS] = {
  "version",
  "version_date",
  "description"
};
#define ELEMENT_DESCRIPTION 2

const char * prefix[NUM_ELEMENTS] = {
  "V:",
  "D:",
  "  ",
};

const char * post[NUM_ELEMENTS] = {
  "\n",
  "\n",
  "\n\n",
};

char query[256];
char buf[256];
char newbuf[256];
int last_space_in, last_space_out;

void main(void) {
  unsigned char err;
  unsigned char i, j, x, n, nn, m;
  int data_len;

  printf("%c#FujiNet Up To Date Checker\n\n", CH_CLR);

  fuji_sio_read_adapter_config();

  printf("You are running version %s\n\n", adapterConfig.fn_version);


  /* Open the JSON file over the network */

  printf("Contacting fujinet.online...");
  err = nopen(1, (char *) url, SIO_READ + SIO_WRITE);
  success_or_fail(err);


  /* Switch to JSON mode */
  err = nchanmode(1, SIO_READ + SIO_WRITE, CHANNELMODE_JSON);
  success_or_fail(err);


  /* Parse the JSON */ 
  printf("parsing...");
  err = njsonparse(1, SIO_READ + SIO_WRITE);
  success_or_fail(err);
  printf("\n\n");

  /* Read the elements */

  for (j = 0; j < 10; j++) {
    for (i = 0; i < NUM_ELEMENTS; i++) {
      sprintf(query, "N1:/releases/%d/%s%c", j,elements[i], CH_EOL);
      err = njsonquery(1, SIO_READ + SIO_WRITE, (char *) query);
      success_or_fail(err);
  
      if (err == 1) {
        err = nstatus(1);
        success_or_fail(err);
  
        if (err == 1) {
          data_len = (OS.dvstat[1] << 8) + OS.dvstat[0];
  
          if (data_len != 0) {
            buf[0] = '\0';
            err = nread(1, buf, data_len);
            success_or_fail(err);
            buf[data_len - 1] = '\0'; /* Remove "\n" at the end! */
  
            if (err == 1) {
              if (i == ELEMENT_DESCRIPTION) {
                last_space_in = -1;
                last_space_out = -1;
                x = strlen(prefix[i]);
                m = 0;
                for (n = 0; n < data_len - 1; n++) {
                  newbuf[m++] = buf[n];
                  x++;
                  if (buf[n] == ' ') {
                    last_space_in = n;
                    last_space_out = m;
                  }
                  if (x == 40 && m - last_space_out + strlen(prefix[i]) < 40) {
                    m = last_space_out - 1;
                    n = last_space_in;
                    newbuf[m++] = '\n';
                    x = 0;
                    for (nn = 0; nn < strlen(prefix[i]); nn++) {
                      newbuf[m++] = ' ';
                      x++;
                    }
                  }
                }
                newbuf[m] = '\0';

                printf("%s%s%s", prefix[i], newbuf, post[i]);
              } else {
                printf("%s%s%s", prefix[i], buf, post[i]);
              }
            }
          } else {
            /* No entry found */
          }
        }
      }
    }
  }

  nclose(1);

  inf_loop();
}

