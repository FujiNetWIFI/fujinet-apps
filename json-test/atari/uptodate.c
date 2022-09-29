/**
 * uptodate.c
 *
 * Connect to fujinet.online to determine the latest official
 * firmware, and ask the #FujiNet what version you're running,
 * and tell you whether you're up-to-date!
 *
 * Bill Kendrick <bill@newbreedsoftware.com>
 *
 * License: v. 3, see LICENSE.md for details
 *
 * 2022-06-01 - 2022-08-09
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

#define IO_CHAN 1

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

/**
 * Spin in an infinite loop
 */
void inf_loop(void) {
  do { } while(1);
}

/**
 * Abort! Show a message, close device, jump to infinite loop
 */
void abort(void) {
  printf("ABORTING\n");
  nclose(IO_CHAN);
  inf_loop();
}

#define ERR_CODE_SUCCESS 1

/**
 * If success, we proceed; if error, we show the error code
 * message and abort the process.
 *
 * @param unsigned char err - Error code
 */
void success_or_fail(unsigned char err) {
  if (err != ERR_CODE_SUCCESS) {
    printf("Error = %d\n", err);
    abort();
  }
}

int output_x = 0;
int output_line = 0;
#define SCREEN_WIDTH 40
#define LINES_UNTIL_PAUSE 20

/**
 * Displays a string of text (which may be more than one line,
 * generally wrapped with explicit newlines (\n)).
 * Once 20 or more lines have been displayed, pauses and prompts
 * the user to press a key (or [Esc] to abort the process).
 *
 * @param char * str - text to display
 */
void show(char * str) {
  int i, len;
  unsigned char c;

  /* Display it... */
  printf(str);

  /* We could possibly use Atari S: screen device
   * values (OS.rowcrs, OS.colcrs), but just managing it
   * ourselves here (more portable? yeah, that's the ticket!) */
  len = strlen(str);
  for (i = 0; i < len; i++) {
    if (str[i] == '\n') {
      output_line++;
      output_x = 0;
    } else {
      output_x++;
      if (output_x == SCREEN_WIDTH) {
        output_x = 0;
        output_line++;
      }
    }
  }

  /* If we're near the bottom of the screen, pause
   * for a keypress.
   */
  if (output_line > LINES_UNTIL_PAUSE && output_x == 0) {
    printf("PRESS A KEY ([Esc] to abort)\n");

    OS.ch = KEY_NONE;
    do {
      c = OS.ch;
    } while (c == KEY_NONE);
    OS.ch = KEY_NONE;

    if (c == KEY_ESC) {
      abort();
    }

    /* Start another chunk of ~20 lines */
    output_line = 0;
  }
}

/* The elements we want to extract from the JSON */
#define NUM_ELEMENTS 3
const char * elements[NUM_ELEMENTS] = {
  "version",
  "version_date",
  "description"
};
#define ELEMENT_VERSION 0
#define ELEMENT_DESCRIPTION 2

/* How to prefix (on-screen) each extracted JSON element we're displaying */
const char * prefix[NUM_ELEMENTS] = {
  "V:",
  "D:",
  "  ",
};

/* How to postfix (on-screen) each extracted JSON element we're displaying */
const char * post[NUM_ELEMENTS] = {
  "\n",
  "\n",
  "\n\n",
};

char query[256];
char buf[256];
char newbuf[256];
char output[256];
int last_space_in, last_space_out;


void main(void) {
  unsigned char err;
  unsigned char i, j, x, n, nn, m;
  int data_len;

  /* Title/credits */
  printf("%c", CH_CLR);
  sprintf(output, "#FujiNet Up-To-Date Checker\nby Bill Kendrick 2022-08-08\n\n");
  show(output);

  /* Read and display our device's version */
  fuji_sio_read_adapter_config();

  sprintf(output, "You are running version %s\n\n", adapterConfig.fn_version);
  show(output);


  /* Open the JSON file that describes available releases (over the network!) */

  sprintf(output, "Contacting fujinet.online...");
  show(output);
  err = nopen(IO_CHAN, (char *) url, SIO_READ + SIO_WRITE);
  success_or_fail(err);


  /* Switch to JSON mode */
  err = nchanmode(IO_CHAN, SIO_READ + SIO_WRITE, CHANNELMODE_JSON);
  success_or_fail(err);


  /* Parse the JSON */
  sprintf(output, "parsing...");
  show(output);
  err = njsonparse(IO_CHAN, SIO_READ + SIO_WRITE);
  success_or_fail(err);
  sprintf(output, "\n\n");
  show(output);

  /* Read the elements */

  for (j = 0; j < 10; j++) {
    for (i = 0; i < NUM_ELEMENTS; i++) {
      sprintf(query, "N1:/releases/%d/%s%c", j, elements[i], CH_EOL);
      err = njsonquery(IO_CHAN, SIO_READ + SIO_WRITE, (char *) query);
      success_or_fail(err);

      if (err == ERR_CODE_SUCCESS) {
        err = nstatus(IO_CHAN);
        success_or_fail(err);

        if (err == ERR_CODE_SUCCESS) {
          data_len = (OS.dvstat[1] << 8) + OS.dvstat[0];

          if (data_len != 0) {
            buf[0] = '\0';
            err = nread(IO_CHAN, buf, data_len);
            success_or_fail(err);
            buf[data_len - 1] = '\0'; /* Remove "\n" at the end! */

            if (err == ERR_CODE_SUCCESS) {
              if (i == ELEMENT_DESCRIPTION) {
                /* Word-wrap this bad-boy */
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

                sprintf(output, "%s%s%s", prefix[i], newbuf, post[i]);
                show(output);
              } else {
                if (i == ELEMENT_VERSION) {
                  if (strcmp(buf, adapterConfig.fn_version + 1 /* skip "#" at the beginning */) == 0) {
                    /* Version from releases JSON is the same as ours! Flag it! */
                    strcpy(buf + strlen(buf), " << YOURS");
                  }
                }

                sprintf(output, "%s%s%s", prefix[i], buf, post[i]);
                show(output);
              }
            }
          } else {
            /* No entry found */
          }
        }
      }
    }
  }

  printf("Done!\n");

  nclose(IO_CHAN);

  inf_loop();
}
