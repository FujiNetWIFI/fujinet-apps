/*
  get_time.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-20
*/

#include <string.h> /* for memset() */
#include <atari.h>
#include "get_time.h"
#include "sio.h"


/* declared in apod.c */
extern unsigned char time_buf[6];
extern unsigned char cur_yr, cur_mo, cur_day, pick_yr, pick_mo, pick_day;

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

