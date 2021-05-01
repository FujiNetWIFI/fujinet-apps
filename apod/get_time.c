/*
  get_time.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module fetches the current date and time (we only
  care about date) from the #FujiNet device.

  If it's unable to, the date "2099-12-31" is set.
  (The user can retry, via the [Ctrl]+[T] option on the
  APOD viewer's main menu.)

  It sets the user's choice of APOD photo date to
  "current" (Y=0, M=0, D=0), which causes the server to
  fetch the photo from the main APOD page, rather than
  a date-stamped archive page.

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-20
*/

#include <string.h> /* for memset() */
#include <atari.h>
#include "get_time.h"
#include "sio.h"

unsigned char last_day[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/**
 * Get time from #FujiNet via APETIME protocol
 * (see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/Accessing-the-Real-Time-Clock)
 */
void get_time(void) {
  memset(time_buf, 0, 6);
  OS.dcb.ddevic = 0x45; /* APETIME protocol */
  OS.dcb.dunit = 0x01; /* unit 1 */
  OS.dcb.dcomnd = 0x93; /* GETTIME request */
  OS.dcb.dstats = 0x40; /* receive */
  OS.dcb.dbuf = (void *) time_buf;
  OS.dcb.dtimlo = 15; /* timeout (seconds) */
  OS.dcb.dunuse = 0;
  OS.dcb.dbyt = (unsigned int) 6; /* reading 6 characters */
  OS.dcb.daux1 = 0xee;
  OS.dcb.daux2 = 0xa0;
  siov();
  if (time_buf[0] != 0) {
    cur_yr = time_buf[2];
    cur_mo = time_buf[1];
    cur_day = time_buf[0];
  } else {
    /* This shouldn't happen */
    cur_yr = 99;
  }
  if (cur_yr >= 99) {
    cur_yr = 99;
    cur_mo = 12;
    cur_day = 31;
  }
  pick_yr = 0;
  pick_mo = 0;
  pick_day = 0;
}
