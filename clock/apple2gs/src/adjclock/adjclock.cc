/**
 * adjclock
 * Temporary initialization file (TIF)
 *
 * @license gpl v. 3, see LICENSE.md for details.
 */

#pragma rtl

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <misctool.h>
#include "fujinet-clock.h"
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

int main(int argc, char **argv)
{
  HexTime fnTime;
  uint8_t fnClock[10];
  uint8_t clock_st;

  MTStartUp();
  sp_init();

  clock_st = clock_get_time(fnClock, SIMPLE_BINARY);
  if (clock_st == FN_ERR_OK)
  {
      fnTime.curYear = 100*fnClock[0]+fnClock[1]-1900;
      fnTime.month   = fnClock[2]-1;
      fnTime.day     = fnClock[3]-1;
      fnTime.hour    = fnClock[4];
      fnTime.minute  = fnClock[5];
      fnTime.second  = fnClock[6];
      WriteTimeHex(fnTime);
  }
  else
    return -1;

  return 0;
}
