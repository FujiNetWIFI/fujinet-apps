/**
 * fnclock
 * ORCA Shell EXE
 *
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <misctool.h>
#include "fujinet-clock.h"
#include "fujinet-network.h"
#include "fujinet-bus-apple2.h"

void usage(void) {
	fputs("usage: fnclock [option(s)]\n", stderr);
	fputs("options:\n", stderr);
	fputs("   -s set the Apple IIgs clock to FujiNet clock\n", stderr);
	fputs("   -? print this option list\n", stderr);
	exit(1);
}

int main(int argc, char **argv)
{
  TimeRec gsTime;
  HexTime fnTime;
  uint8_t fnClock[10];
  uint8_t clock_st;
  unsigned i;
  unsigned flag_s = 0;

	for (i = 1; i<argc; ++i) {
    if (argv[i][0] != '-') usage();
		switch(argv[i][1]) {
			case 's': flag_s = 1; break;
			default: usage();
		}
	}
  
  MTStartUp();
  sp_init();

  clock_st = clock_get_time(fnClock, SIMPLE_BINARY);
  if (clock_st == FN_ERR_OK)
  {
    if (flag_s)
    {
      fnTime.curYear = 100*fnClock[0]+fnClock[1]-1900;
      fnTime.month   = fnClock[2]-1;
      fnTime.day     = fnClock[3]-1;
      fnTime.hour    = fnClock[4];
      fnTime.minute  = fnClock[5];
      fnTime.second  = fnClock[6];
      WriteTimeHex(fnTime);
      printf("Current date & time adjusted to the FujiNet clock.\n\n");
    }

    gsTime = ReadTimeHex();
    printf("Apple IIgs  Date: %04u-%02u-%02u\n            Time: %02u:%02u:%02u\n",
      gsTime.year+1900,
      gsTime.month+1,
      gsTime.day+1,
      gsTime.hour,
      gsTime.minute,
      gsTime.second);

    printf("\nFujiNet     Date: %02u%02u-%02u-%02u\n            Time: %02u:%02u:%02u\n",
      fnClock[0], // Century
      fnClock[1], // Year
      fnClock[2], // Month
      fnClock[3], // Day
      fnClock[4], // Hour
      fnClock[5], // Minute
      fnClock[6]); // Second
  }
  else
  {
    printf("Cannot get FujiNet time - err %d\n", clock_st);
    return -1;
  }

  return 0;
}
