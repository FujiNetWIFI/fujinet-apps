/*
  dlists.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-05-01
*/

#include <stdio.h>
#include <atari.h>
#include "screen_helpers.h"

/* A block of space to store the graphics & display lists */
extern unsigned char scr_mem[];


/**
 * Set up a basic Display List
 *
 * @param byte antic_mode
 */
void dlist_setup(unsigned char antic_mode) {
  unsigned char i;
  unsigned int gfx_ptr;

  screen_off();

  gfx_ptr = (unsigned int) (scr_mem + SCR_OFFSET);

  dlist1[0] = DL_BLK8;
  dlist1[1] = DL_BLK8;
  dlist1[2] = DL_BLK8;

  /* Row 0 */
  dlist1[3] = DL_LMS(antic_mode);
  dlist1[4] = (gfx_ptr & 255);
  dlist1[5] = (gfx_ptr >> 8);

  for (i = 6; i <= 106; i++) {
    dlist1[i] = antic_mode;
  }

  /* Hitting 4K boundary! */
  gfx_ptr = (unsigned int) (scr_mem + 4096);
  dlist1[107] = DL_LMS(antic_mode);
  dlist1[108] = (gfx_ptr & 255);
  dlist1[109] = (gfx_ptr >> 8);

  for (i = 110; i <= 198; i++) {
    dlist1[i] = antic_mode;
  }

  dlist1[199] = DL_JVB;
  dlist1[200] = ((unsigned int) dlist1 & 255);
  dlist1[201] = ((unsigned int) dlist1 >> 8);

  screen_on();
}


/**
 * Set up three Display Lists for ColorView 9 & 15 RGB modes
 * (utilizes DLI and VBI to flicker; see above)
 *
 * @param byte antic_mode
 */
void dlist_setup_rgb(unsigned char antic_mode, unsigned char occasional_dli) {
  int l, i;
  unsigned int gfx_ptr /*, next_dlist */;
  unsigned char * dlist;

  screen_off();

  for (l = 0; l < 3; l++) {
    dlist = (scr_mem + (l * SCR_BLOCK_SIZE)) + DLIST_OFFSET;
    gfx_ptr = (unsigned int) (scr_mem + (l * SCR_BLOCK_SIZE)) + SCR_OFFSET;

    dlist[0] = DL_BLK8;
    dlist[1] = DL_BLK8;
    dlist[2] = DL_DLI(DL_BLK8); /* start with colors after this line */

    /* Row 0 */
    dlist[3] = DL_LMS(antic_mode);
    dlist[4] = (gfx_ptr & 255);
    dlist[5] = (gfx_ptr >> 8);

    for (i = 6; i <= 106; i++) {
      if (occasional_dli && (i % 6) == 4) {
        dlist[i] = DL_DLI(antic_mode);
      } else {
        dlist[i] = antic_mode;
      }
    }

    /* Hitting 4K boundary! */
    gfx_ptr += (102 * 40);
    dlist[107] = DL_LMS(antic_mode);
    dlist[108] = (gfx_ptr & 255);
    dlist[109] = (gfx_ptr >> 8);

    for (i = 110; i <= 197; i++) {
      if (occasional_dli && i % 6 == 0) {
        dlist[i] = DL_DLI(antic_mode);
      } else {
        dlist[i] = antic_mode;
      }
    }
    dlist[198] = antic_mode;

    dlist[199] = DL_JVB;
    dlist[200] = (((unsigned int) dlist) & 255);
    dlist[201] = (((unsigned int) dlist) >> 8);
  }

  screen_on();
}


/**
 * Set up two Display Lists for APAC 256 color mode
 * (utilizes DLI and VBI to flicker; see above)
 *
 * @param byte antic_mode
 */
void dlist_setup_apac(void) {
  int i;
  unsigned char * dlist1, * dlist2;
  unsigned int gfx_ptr1, gfx_ptr2;

  screen_off();

  dlist1 = scr_mem + DLIST_OFFSET;
  dlist2 = dlist1 + SCR_BLOCK_SIZE;
  gfx_ptr1 = (unsigned int) (scr_mem + SCR_OFFSET);
  gfx_ptr2 = (unsigned int) (gfx_ptr1 + SCR_BLOCK_SIZE);

  dlist1[0] = DL_BLK8;
  dlist1[1] = DL_BLK8;
  dlist1[2] = DL_DLI(DL_BLK8); /* start with colors after this line */

  dlist2[0] = DL_BLK8;
  dlist2[1] = DL_BLK8;
  dlist2[2] = DL_BLK8;

  /* Row 0 */
  dlist1[3] = DL_LMS(DL_GRAPHICS8);
  dlist1[4] = (gfx_ptr1 & 255);
  dlist1[5] = (gfx_ptr1 >> 8);

  dlist2[3] = DL_LMS(DL_DLI(DL_GRAPHICS8));
  dlist2[4] = (gfx_ptr2 & 255);
  dlist2[5] = (gfx_ptr2 >> 8);

  for (i = 6; i <= 106; i+=2) {
    dlist1[i] = DL_DLI(DL_GRAPHICS8);
    dlist1[i + 1] = DL_GRAPHICS8;

    dlist2[i] = DL_GRAPHICS8;
    dlist2[i + 1] = DL_DLI(DL_GRAPHICS8);
  }

  /* Hitting 4K boundary! */
  gfx_ptr1 += (102 * 40);
  gfx_ptr2 += (102 * 40);

  dlist1[107] = DL_LMS(DL_GRAPHICS8);
  dlist1[108] = (gfx_ptr1 & 255);
  dlist1[109] = (gfx_ptr1 >> 8);

  dlist2[107] = DL_LMS(DL_DLI(DL_GRAPHICS8));
  dlist2[108] = (gfx_ptr2 & 255);
  dlist2[109] = (gfx_ptr2 >> 8);

  for (i = 110; i <= 198; i+=2) {
    dlist1[i] = DL_DLI(DL_GRAPHICS8);
    dlist1[i + 1] = DL_GRAPHICS8;

    dlist2[i] = DL_GRAPHICS8;
    dlist2[i + 1] = DL_DLI(DL_GRAPHICS8);
  }

  dlist1[199] = DL_JVB;
  dlist1[200] = (((unsigned int) dlist1) & 255);
  dlist1[201] = (((unsigned int) dlist1) >> 8);

  dlist2[199] = DL_JVB;
  dlist2[200] = (((unsigned int) dlist2) & 255);
  dlist2[201] = (((unsigned int) dlist2) >> 8);

  screen_on();
}
