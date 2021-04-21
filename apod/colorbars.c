/*
  colorbars.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-21
*/

#include <string.h> /* for memset() */
#include "screen_helpers.h"
#include "colorbars.h"

void render_colorbars(void) {
  int i;
  unsigned char r, g, b, grey;

  for (i = 0; i < 40; i++) {
    r = (i >= 34 || i < 14) ? 0x55 : 0;
    g = (6 < i && i < 28) ? 0x55 : 0;
    b = (20 < i) ? 0x55 : 0;

    scr_mem1[i +  0] = r;
    scr_mem1[i + 40] = g;
    scr_mem1[i + 80] = b;

    scr_mem2[i +  0] = g;
    scr_mem2[i + 40] = b;
    scr_mem2[i + 80] = r;

    scr_mem3[i +  0] = b;
    scr_mem3[i + 40] = r;
    scr_mem3[i + 80] = g;
  }
  for (i = 120; i < 5120; i += 120) {
    memcpy(scr_mem1 + i, scr_mem1, 120);
    memcpy(scr_mem2 + i, scr_mem2, 120);
    memcpy(scr_mem3 + i, scr_mem3, 120);
  }

  /* 16 shades of grey */
  for (i = 5120; i < 5160; i++) {
    grey = ((i % 40) << 1) / 5;
    grey = grey * 17;
    scr_mem1[i] = grey;
  }
  for (i = 5160; i < 6400; i += 40) {
    memcpy(scr_mem1 + i, scr_mem1 + 5120, 40);
  }

  /* 8 shades of grey */
  for (i = 6400; i < 6440; i++) {
    grey = ((i % 40) / 5) << 1;
    grey = grey * 17;
    scr_mem1[i] = grey;
  }
  for (i = 6440; i < 7680; i += 40) {
    memcpy(scr_mem1 + i, scr_mem1 + 6400, 40);
  }

  memcpy(scr_mem2 + 5120, scr_mem1 + 5120, 2560);
  memcpy(scr_mem3 + 5120, scr_mem1 + 5120, 2560);
}
