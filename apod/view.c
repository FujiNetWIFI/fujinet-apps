/*
  view.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-24
*/

#include <stdio.h>
#include <atari.h>
#include "colorbars.h"
#include "dli9.h"
#include "dli256.h"
#include "fetch.h"
#include "interrupt_helpers.h"
#include "menu.h"
#include "rgb.h"
#include "screen_helpers.h"
#include "view.h"

/* A block of space to store the graphics & display lists */
extern unsigned char scr_mem[];


/* FIXME: Further modularize display list stuff */

/**
 * Set up a basic Display List
 *
 * @param byte antic_mode
 */
void dlist_setup(unsigned char antic_mode) {
  unsigned char i;
  unsigned int gfx_ptr, dlist_idx;

  screen_off();

  gfx_ptr = (unsigned int) (scr_mem + SCR_OFFSET);

  dlist_idx = 0;

  dlist1[dlist_idx++] = DL_BLK8;
  dlist1[dlist_idx++] = DL_BLK8;
  dlist1[dlist_idx++] = DL_BLK8;

  /* Row 0 */
  dlist1[dlist_idx++] = DL_LMS(antic_mode);
  dlist1[dlist_idx++] = (gfx_ptr & 255);
  dlist1[dlist_idx++] = (gfx_ptr >> 8);

  for (i = 1; i <= 101; i++) {
    dlist1[dlist_idx++] = antic_mode;
  }

  /* Hitting 4K boundary! */
  i++;
  gfx_ptr = (unsigned int) (scr_mem + 4096);
  dlist1[dlist_idx++] = DL_LMS(antic_mode);
  dlist1[dlist_idx++] = (gfx_ptr & 255);
  dlist1[dlist_idx++] = (gfx_ptr >> 8);

  for (i = i; i <= 191; i++) {
    dlist1[dlist_idx++] = antic_mode;
  }

  dlist1[dlist_idx++] = DL_JVB;
  dlist1[dlist_idx++] = ((unsigned int) dlist1 & 255);
  dlist1[dlist_idx++] = ((unsigned int) dlist1 >> 8);

  screen_on();
}


/* Tracking which Display List is active */
unsigned char dlist_hi, dlist_lo;

/* Keeping track of which RGB color we're showing
   (for fetching from the look-up table),
   and APAC frame */
unsigned char rgb_ctr, apac_scanline;


#pragma optimize (push, off)

/* VBI routine for flipping between our three
   Display Lists in RGB image modes */
void VBLANKD9(void) {
  /* grab the current rgb color counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 3 back to 0 */
  asm("inx");
  asm("cpx #3");
  asm("bcc %g", __vbi9_ctr_set);

  asm("ldx #0");

__vbi9_ctr_set:
  /* store the current rgb color counter back;
     also store it as a reference to our next display list */
  asm("stx %v", dli9_load_arg);
  asm("stx %v", rgb_ctr);

  /* display lists are 8K away from each other
     (tucked under screen memory); that's 32 (256 byte) pages,
     so we can shift left 5 times to multiply the rgb color counter
     by 32... then store it in the high byte of SDLST */
  asm("txa");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("adc %v", dlist_hi);
  asm("sta $d403");
  asm("lda %v", dlist_lo);
  asm("sta $d402");

  /* adjust end of the screen colors - set the last one to black color */
  asm("lda %v+187,x", rgb_table);
  asm("sta %v+190,x", rgb_table);
  asm("lda %v+188,x", rgb_table);
  asm("sta %v+191,x", rgb_table);
  asm("lda #0");
  asm("sta %v+192,x", rgb_table);

  /* start next screen with black color at top */
  asm("sta $d01a");

  asm("jmp (%v)", OLDVEC);
}

/* VBI routine for flipping between our two
   Display Lists in APAC image modes */
void VBLANKD_APAC(void) {
  /* grab the current counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 2 back to 0 */
  asm("inx");
  asm("cpx #2");
  asm("bcc %g", __vbi256_ctr_set);

  asm("ldx #0");

__vbi256_ctr_set:
  /* store the current counter back;
     also store it as a reference to our next display list */
  asm("stx %v", rgb_ctr);

  /* display lists are 8K away from each other
     (tucked under screen memory); that's 32 (256 byte) pages,
     so we can shift left 5 times to multiply the rgb color counter
     by 32... then store it in the high byte of SDLST */
  asm("txa");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("adc %v", dlist_hi);
  asm("sta $d403");
  asm("lda %v", dlist_lo);
  asm("sta $d402");

  asm("jmp (%v)", OLDVEC);
}

#pragma optimize (pop)


/**
 * Set up three Display Lists for ColorView 9 RGB mode
 * (utilizes DLI and VBI to flicker; see above)
 *
 * @param byte antic_mode
 */
void dlist_setup_rgb9(unsigned char antic_mode) {
  int l, i;
  unsigned int gfx_ptr /*, next_dlist */;
  unsigned int dlist_idx;
  unsigned char * dlist;

  screen_off();

  for (l = 0; l < 3; l++) {
    dlist = (scr_mem + (l * SCR_BLOCK_SIZE)) + DLIST_OFFSET;
    gfx_ptr = (unsigned int) (scr_mem + (l * SCR_BLOCK_SIZE)) + SCR_OFFSET;

    dlist_idx = 0;

    dlist[dlist_idx++] = DL_BLK8;
    dlist[dlist_idx++] = DL_BLK8;
    dlist[dlist_idx++] = DL_DLI(DL_BLK8); /* start with colors after this line */

    /* Row 0 */
    dlist[dlist_idx++] = DL_LMS(DL_DLI(antic_mode));
    dlist[dlist_idx++] = (gfx_ptr & 255);
    dlist[dlist_idx++] = (gfx_ptr >> 8);

    for (i = 1; i <= 101; i++) {
      dlist[dlist_idx++] = DL_DLI(antic_mode);
    }

    /* Hitting 4K boundary! */
    gfx_ptr += (102 * 40);
    dlist[dlist_idx++] = DL_LMS(DL_DLI(antic_mode));
    dlist[dlist_idx++] = (gfx_ptr & 255);
    dlist[dlist_idx++] = (gfx_ptr >> 8);

    for (i = 103; i <= 191; i++) {
      dlist[dlist_idx++] = DL_DLI(antic_mode);
    }

    dlist[dlist_idx++] = DL_JVB;
    dlist[dlist_idx++] = (((unsigned int) dlist) & 255);
    dlist[dlist_idx++] = (((unsigned int) dlist) >> 8);
  }

  setup_rgb_table();

  screen_on();
}

/**
 * Set up three Display Lists for ColorView 15 RGB mode
 * (utilizes DLI and VBI to flicker; see above)
 *
 * @param byte antic_mode
 */
void dlist_setup_rgb15(unsigned char antic_mode) {
  int l, i;
  unsigned int gfx_ptr /*, next_dlist */;
  unsigned int dlist_idx;
  unsigned char * dlist;

  screen_off();

  for (l = 0; l < 3; l++) {
    dlist = (scr_mem + (l * SCR_BLOCK_SIZE)) + DLIST_OFFSET;
    gfx_ptr = (unsigned int) (scr_mem + (l * SCR_BLOCK_SIZE)) + SCR_OFFSET;

    dlist_idx = 0;

    dlist[dlist_idx++] = DL_BLK8;
    dlist[dlist_idx++] = DL_BLK8;
    dlist[dlist_idx++] = DL_DLI(DL_BLK8); /* start with colors after this line */

    /* Row 0 */
    dlist[dlist_idx++] = DL_LMS(DL_DLI(antic_mode));
    dlist[dlist_idx++] = (gfx_ptr & 255);
    dlist[dlist_idx++] = (gfx_ptr >> 8);

    for (i = 1; i <= 101; i++) {
      if (i % 3 == 0) {
        dlist[dlist_idx++] = DL_DLI(antic_mode);
      } else {
        dlist[dlist_idx++] = DL_DLI(antic_mode);
      }
    }

    /* Hitting 4K boundary! */
    gfx_ptr += (102 * 40);
    dlist[dlist_idx++] = DL_LMS(DL_DLI(antic_mode));
    dlist[dlist_idx++] = (gfx_ptr & 255);
    dlist[dlist_idx++] = (gfx_ptr >> 8);

    for (i = 103; i <= 191; i++) {
      if (i % 3 == 0) {
        dlist[dlist_idx++] = DL_DLI(antic_mode);
      } else {
        dlist[dlist_idx++] = DL_DLI(antic_mode);
      }
    }

    dlist[dlist_idx++] = DL_DLI(DL_BLK1);

    dlist[dlist_idx++] = DL_JVB;
    dlist[dlist_idx++] = (/*next_*/((unsigned int) dlist) & 255);
    dlist[dlist_idx++] = (/*next_*/((unsigned int) dlist) >> 8);
  }

  setup_rgb_table();

  screen_on();
}


/**
 * Set up three Display Lists for APAC 256 color mode
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


/**
 * Fetch or render the image, and view it!
 */
void view(unsigned char choice, char sample, unsigned char pick_yr, unsigned pick_mo, unsigned pick_day) {
  int size;
  unsigned char done, k, interrupts_used;

  /* Set up the display, based on the choice */
  size = 7680;

  OLDVEC = OS.vvblkd;

  /* Prepare display for loading image... */
  if (choice == CHOICE_HIRES_MONO) {
    dlist_setup(DL_GRAPHICS8);
    OS.color4 = 128; /* Border (no reason) */
    OS.color2 = 0; /* Background */
    OS.color1 = 15; /* Foreground */
  } else if (choice == CHOICE_LOWRES_GREY) {
    dlist_setup(DL_GRAPHICS8);
    OS.gprior = 64;
    OS.color4 = 0; /* Greyscale */
  } else if (choice == CHOICE_MEDRES_COLOR) {
    dlist_setup(DL_GRAPHICS15);
    /* (Colors will be replaced by palette fetched from server) */
    OS.color4 = 0; /* Background (black) */
    OS.color0 = 4; /* Dark foreground */
    OS.color1 = 8; /* Medium foreground */
    OS.color2 = 14; /* Light foreground */
  } else if (choice == CHOICE_LOWRES_RGB) {
    size = 7680 * 3;
    dlist_setup_rgb9(DL_GRAPHICS8);
    OS.gprior = 64;
  } else if (choice == CHOICE_MEDRES_RGB) {
    size = 7680 * 3;
    dlist_setup_rgb15(DL_GRAPHICS15);
  } else if (choice == CHOICE_LOWRES_256) {
    size = 7680 * 2;
    dlist_setup_apac();
  }


  /* Load or render the image! */
  wait_for_vblank();
  if (sample == SAMPLE_COLORBARS) {
    render_colorbars(choice);
  } else {
    fetch_image(choice, sample, size, pick_yr, pick_mo, pick_day);
  }

  dlist_hi = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) >> 8);
  dlist_lo = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) & 255);

  /* Enable interrupt-driven graphics mode, if applicable */
  if (choice == CHOICE_LOWRES_RGB) {
    rgb_ctr = 0;
    mySETVBV((void *) VBLANKD9);
    dli_init(dli9);
    interrupts_used = 1;
  } else if (choice == CHOICE_MEDRES_RGB) {
    // mySETVBV((void *) VBLANKD); /* FIXME */
    // dli_init(dli); /* FIXME */
    // interrupts_used = 1;
  } else if (choice == CHOICE_LOWRES_256) {
    mySETVBV((void *) VBLANKD_APAC);
    dli_init(dli256);
    interrupts_used = 1;
  } else {
    interrupts_used = 0;
  }


  /* Accept keypresses while viewing */
  OS.ch = KEY_NONE;
  done = 0;
  do {
    k = OS.ch;
    if (k == KEY_ESC) {
      done = 1;
    } else if ((k & 0x3F) == KEY_R || (k & 0x3F) == KEY_G || (k & 0x3F) == KEY_B || k == KEY_X) {
      /* [R], [G], or [B] key, with our without [Shift] */
      handle_rgb_keypress(k);
      setup_rgb_table();
      OS.ch = KEY_NONE;
    }
  } while (!done);
  OS.ch = KEY_NONE;

  /* Reset things, and return to main loop */
  if (interrupts_used) {
    dli_clear();
    mySETVBV((void *) OLDVEC);
  }
  OS.gprior = 0;
}
