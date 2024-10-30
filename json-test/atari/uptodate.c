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
 * 2022-06-01 - 2024-10-29
 */

// #define TEST_MODE

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sio.h"
#include "nsio.h"
#include "../../../fujinet-lib/fujinet-fuji.h"

/* https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24E8-Get-Adapter-Config */
/* The struct (defined in `fujinet-lib`) holds the response to SIO_FUJICMD_GET_ADAPTERCONFIG */
AdapterConfig adapterConfig;

void fuji_sio_read_adapter_config()
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dcomnd = 0xE8; /* Get Adapter Config ($E8) "FUJICMD_GET_ADAPTERCONFIG" */
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = &adapterConfig;
  OS.dcb.dtimlo = 0x0f;
  OS.dcb.dbyt = sizeof(adapterConfig);
  OS.dcb.daux = 0;
  siov();

  /* `fujinet-platformio/lib/device/sio/fuji.cpp` runs the function
     `sioFuji::sio_get_adapter_config()` in response to this request.

     As of 2023-05-12 that, in turn, calls `fnSystem.get_fujinet_version(true)`
     (inside `lib/hardware/fnSystem.cpp`) which, due to `shortVersionOnly`
     argument being true, simply returns `FN_VERSION_FULL`, which is
     #define'd in `include/version.h`.
  */
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

char dlist[36];
char top_text[160];

void top_show(int y, char * str) {
  int i, yy;
  unsigned char c;

  yy = y * 40;
  for (i = 0; str[i] != '\0'; i++) {
    c = str[i];
    if (c == CH_EOL) {
      continue;
    }
    if (c >= 128) {
      c = c - 128;
    }
    if (c < 32) {
      top_text[yy + i] = c + 64 + 128;
    } else if (c < 96) {
      top_text[yy + i] = c - 32 + 128;
    } else {
      top_text[yy + i] = c + 128;
    }
  }
}

void main(void) {
  unsigned char err;
  unsigned char i, j, x, n, nn, m;
  int data_len;

  printf("%c", CH_CLR);

  memset(top_text, 128, 160);

  dlist[0] = DL_BLK8;
  dlist[1] = DL_BLK8;
  dlist[2] = DL_BLK8;
  dlist[3] = DL_LMS(DL_CHR40x8x1);
  dlist[4] = ((unsigned int) top_text) & 0xFF;
  dlist[5] = ((unsigned int) top_text) >> 8;
  dlist[6] = DL_CHR40x8x1;
  dlist[7] = DL_CHR40x8x1;
  dlist[8] = DL_CHR40x8x1;
  dlist[9] = DL_BLK1;
  dlist[10] = DL_LMS(DL_CHR40x8x1);
  dlist[11] = ((unsigned int) (OS.savmsc + 160)) & 0xFF;
  dlist[12] = ((unsigned int) (OS.savmsc + 160)) >> 8;
  for (i = 13; i < 33; i++) {
    dlist[i] = DL_CHR40x8x1;
  }
  dlist[33] = DL_JVB;
  dlist[34] = ((unsigned int) dlist) & 0xFF;
  dlist[35] = ((unsigned int) dlist) >> 8;

  OS.sdlst = dlist;

  /* Title/credits */

  for (i = 0; i < 4; i++) {
    sprintf(output, "%d\n", i);
    show(output);
  }

  strcpy(output, "#FujiNet Up-To-Date Checker");
  top_show(0, output);
  strcpy(output, "by Bill Kendrick 2024-10-29");
  top_show(1, output);

  /* Read and display our device's version */
#ifdef TEST_MODE
  strcpy(output, "Test mode");
#else
  fuji_sio_read_adapter_config();
  sprintf(output, "You are running version %s\n", adapterConfig.fn_version);
#endif
  top_show(3, output);


  /* Open the JSON file that describes available releases (over the network!) */

  sprintf(output, "Contacting fujinet.online...");
  show(output);

#ifndef TEST_MODE
  err = nopen(IO_CHAN, (char *) url, SIO_READ + SIO_WRITE);
  success_or_fail(err);
#endif


  /* Switch to JSON mode */
#ifndef TEST_MODE
  err = nchanmode(IO_CHAN, SIO_READ + SIO_WRITE, CHANNELMODE_JSON);
  success_or_fail(err);
#endif

  /* Parse the JSON */
  strcpy(output, "parsing...");
  show(output);

#ifndef TEST_MODE
  err = njsonparse(IO_CHAN, SIO_READ + SIO_WRITE);
  success_or_fail(err);
#endif

  strcpy(output, "\n\n");
  show(output);

  /* Read the elements */

#ifndef TEST_MODE
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
#endif

  strcpy(output, "Done!\n");
  show(output);

#ifndef TEST_MODE
  nclose(IO_CHAN);
#endif

  inf_loop();
}
