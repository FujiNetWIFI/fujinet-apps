/*
  colorbars.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module renders a test pattern ("color bars").
  Different screen modes get different patterns.

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-05-04
*/

#include <string.h> /* for memset() */
#include "screen_helpers.h"
#include "colorbars.h"
#include "menu.h"

#include <atari_screen_charmap.h>
#define COLORBARS_TXT "COLORBARS"

/**
 * Render the appropriate test pattern ("color bars")
 * for the given mode.
 *
 * @param byte mode -- one of the viewing modes described in "menu.h"
 */
void render_colorbars(unsigned char mode) {
  int i, i2, i3, j;
  unsigned char r, g, b, grey, hue;

  if (mode == CHOICE_HIRES_MONO) {
    /* Monochrome dither pattern simulating shades */
    for (i = 8; i < 40; i++) {
      scr_mem1[i] = (i < 16) ? 0x88 : (i < 32) ? 0x55 : 0xff;
      scr_mem1[i + 40] = (i < 16) ? 0 : (i < 24) ? 0xaa : 0xff;
    }
    for (i = 80; i < 7680; i += 80) {
      memcpy(scr_mem1 + i, scr_mem1, 80);
    }
  } else if (mode == CHOICE_MEDRES_COLOR) {
    /* Four grey scale shades */
    for (i = 10; i < 40; i++) {
      scr_mem1[i] = (i < 20) ? 0x55 : (i < 30) ? 0xaa : 0xff;
    }
    for (i = 40; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
    }
  } else if (mode == CHOICE_LOWRES_GREY) {
    /* Sixteen grey scale shades */
    for (i = 0; i < 40; i++) {
      grey = ((i << 1) / 5) * 17;
      scr_mem1[i] = grey;
    }
    for (i = 40; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
    }
  } else if (mode == CHOICE_LOWRES_RGB) {
    /* TV-style color bars (for 4096 color mode) */

    /* 6 colors at the top: Red, R+G=Yellow, Green, G+B=Cyan, Blue, and B+R=Purple */
    for (i = 0; i < 40; i++) {
      i2 = i + 40;
      i3 = i + 80;

      r = (i >= 34 || i < 14) ? 0x55 : 0;
      g = (6 < i && i < 28) ? 0x55 : 0;
      b = (20 < i) ? 0x55 : 0;

      scr_mem1[i] = r;
      scr_mem1[i2] = g;
      scr_mem1[i3] = b;

      scr_mem2[i] = g;
      scr_mem2[i2] = b;
      scr_mem2[i3] = r;

      scr_mem3[i] = b;
      scr_mem3[i2] = r;
      scr_mem3[i3] = g;
    }
    for (i = 120; i < 5120; i += 120) {
      memcpy(scr_mem1 + i, scr_mem1, 120);
      memcpy(scr_mem2 + i, scr_mem2, 120);
      memcpy(scr_mem3 + i, scr_mem3, 120);
    }

    /* 16 shades of grey at the bottom */
    for (i = 0; i < 40; i++) {
      grey = ((i << 1) / 5) * 17;
      scr_mem1[i + 5120] = grey;
    }
    for (i = 5160; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1 + 5120, 40);
    }

    for (i = 6440; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1 + 6400, 40);
    }

    memcpy(scr_mem2 + 5120, scr_mem1 + 5120, 2560);
    memcpy(scr_mem3 + 5120, scr_mem1 + 5120, 2560);
  } else if (mode == CHOICE_MEDRES_RGB) {
    /* TV-style color bars (for 64 color mode) */

    /* 6 colors at the top: Red, R+G=Yellow, Green, G+B=Cyan, Blue, and B+R=Purple */
    for (i = 0; i < 40; i++) {
      i2 = i + 40;
      i3 = i + 80;

      r = (i >= 34 || i < 14) ? 0x55 : 0;
      g = (6 < i && i < 28) ? 0x55 : 0;
      b = (20 < i) ? 0x55 : 0;

      scr_mem1[i] = r;
      scr_mem1[i2] = g;
      scr_mem1[i3] = b;

      scr_mem2[i] = g;
      scr_mem2[i2] = b;
      scr_mem2[i3] = r;

      scr_mem3[i] = b;
      scr_mem3[i2] = r;
      scr_mem3[i3] = g;
    }
    for (i = 120; i < 5120; i += 120) {
      memcpy(scr_mem1 + i, scr_mem1, 120);
      memcpy(scr_mem2 + i, scr_mem2, 120);
      memcpy(scr_mem3 + i, scr_mem3, 120);
    }

    /* 4 shades of grey at the bottom */
    for (i = 0; i < 40; i++) {
      grey = (i < 10 ? 0 : (i < 20 ? 85 : (i < 30 ? 170 : 255)));
      scr_mem1[i + 5120] = grey;
    }
    for (i = 5160; i < 7680; i += 40) {
      memcpy(scr_mem1 + i, scr_mem1 + 5120, 40);
    }

    memcpy(scr_mem2 + 5120, scr_mem1 + 5120, 2560);
    memcpy(scr_mem3 + 5120, scr_mem1 + 5120, 2560);
  } else if (mode == CHOICE_LOWRES_256) {
    /* 256-color grid
       (simulates the well-known "16-shade mode with 16 hues applied via
       Display List Interrupt" effect, but rendered as Any Point Any Color
       (APAC) interlaced flickering mode) */
    for (i = 0; i < 40; i++) {
      grey = ((i << 1) / 5) * 17;
      scr_mem1[i] = grey;
      scr_mem2[i + 40] = grey;
    }
    for (i = 80; i < 7680; i += 80) {
      memcpy(scr_mem1 + i, scr_mem1, 40);
      memcpy(scr_mem2 + i + 40, scr_mem2 + 40, 40);
    }
    hue = 0;
    for (i = 0; i < 192; i += 12) {
      i2 = i * 40;
      for (j = 0; j < 40; j++) {
        scr_mem1[i2 + (j + 40)] = hue;
        scr_mem2[i2 + j] = hue;
      }
      hue += 17;
      for (j = 2; j < 12; j += 2) {
        memcpy(scr_mem1 + (i + j + 1) * 40, scr_mem1 + (i2 + 40), 40);
        memcpy(scr_mem2 + (i + j) * 40, scr_mem2 + i2, 40);
      }
    }
  }

  memcpy(txt_mem, COLORBARS_TXT, strlen(COLORBARS_TXT));
}
