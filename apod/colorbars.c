/*
  colorbars.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-24
*/

#include <string.h> /* for memset() */
#include "screen_helpers.h"
#include "colorbars.h"
#include "menu.h"

void render_colorbars(unsigned char mode) {
  int i, j;
  unsigned char r, g, b, grey, hue;

  if (mode == CHOICE_HIRES_MONO) {
    for (i = 0; i < 40; i++) {
      scr_mem1[i] = (i < 8) ? 0 : (i < 16) ? 0x88 : (i < 32) ? 0x55 : 0xff;
      scr_mem1[i + 40] = (i < 16) ? 0 : (i < 24) ? 0xaa : 0xff;
    }
    for (i = 80; i < 7680; i += 80) {
      memcpy(scr_mem1 + i, scr_mem1, 80);
    }
  } else if (mode == CHOICE_MEDRES_COLOR) {
    for (i = 0; i < 40; i++) {
      scr_mem1[i] = (i < 10) ? 0 : (i < 20) ? 0x55 : (i < 30) ? 0xaa : 0xff;
    }
    for (i = 40; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
    }
  } else if (mode == CHOICE_LOWRES_GREY) {
    for (i = 0; i < 40; i++) {
      grey = (i << 1) / 5;
      grey = grey * 17;
      scr_mem1[i] = grey;
    }
    for (i = 40; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
    }
  } else if (mode == CHOICE_LOWRES_RGB) {
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
  } else if (mode == CHOICE_MEDRES_RGB) {
    /* FIXME */
  } else if (mode == CHOICE_LOWRES_256) {
    for (i = 0; i < 40; i++) {
      grey = (i << 1) / 5;
      grey = grey * 17;
      scr_mem1[i] = grey;
      scr_mem2[i + 40] = grey;
    }
    for (i = 80; i < 7680; i += 80) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
      memcpy(scr_mem2 + i + 40, scr_mem2 + 40, 40);
    }
    hue = 0;
    for (i = 0; i < 192; i += 12) {
      for (j = 0; j < 40; j++) {
        scr_mem1[(i * 40) + (j + 40)] = hue;
        scr_mem2[(i * 40) + j] = hue;
      }
      hue += 17;
      for (j = 2; j < 12; j += 2) {
        memcpy(scr_mem1 + (i + j + 1) * 40, scr_mem1 + ((i + 1) * 40), 40);
        memcpy(scr_mem2 + (i + j) * 40, scr_mem2 + (i * 40), 40);
      }
    }
  }
}
