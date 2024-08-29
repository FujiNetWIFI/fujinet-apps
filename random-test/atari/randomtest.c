/**
 * randomtest.c
 * 
 * Tests the receiption of a Fujinet random number
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sio.h"


short fuji_random(void)
{
  unsigned char buf[10];
  short num;
  char *p;

  memset(buf, 0, sizeof(buf));

	OS.dcb.ddevic = 0x70;
	OS.dcb.dunit  = 0x00;
	OS.dcb.dcomnd = 0xD3;
	OS.dcb.dstats = 0x0C;
	OS.dcb.dbuf   = buf;
	OS.dcb.dtimlo = 0x0F;
	OS.dcb.dbyt   = 0x00;
	OS.dcb.daux   = 0x00;
	siov();

  p = &num;

  p[0] = buf[0];
  p[1] = buf[1];

  return num;
}

void main(void) {
  short num;
  char wait[10];

  printf("Random Test " __DATE__  "  " __TIME__ "\n\n");

  num = fuji_random();
  printf("num: %d $04x\n", num, num);

  gets(wait);
}
