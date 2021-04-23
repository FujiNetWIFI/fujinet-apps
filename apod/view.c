/*
  view.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-22
*/

#include <stdio.h>
#include <atari.h>
#include "colorbars.h"
#include "dli9.h"
#include "interrupt_helpers.h"
#include "menu.h"
#include "nsio.h"
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
   (for fetching from the look-up table) */
unsigned char rgb_ctr;


/* VBI routine for flipping between our three
   Display Lists in RGB image modes */
#pragma optimize (push, off)
void VBLANKD9(void) {
  /* grab the current rgb color counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 3 back to 0 */
  asm("inx");
  asm("cpx #3");
  asm("bcc %g", __vbi_ctr_set);

  asm("ldx #0");

__vbi_ctr_set:
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


/* The base URL for the web app */
char * baseurl = "N:HTTP://billsgames.com/fujinet/apod/index.php";

/* Space to store the composed URL */
char url[255];


/**
 * Fetch or render the image, and view it!
 */
void view(unsigned char choice, char sample, unsigned char pick_yr, unsigned pick_mo, unsigned pick_day) {
  int size;
  unsigned short data_len, data_read;
  unsigned char done, k;

  /* Set up the display, based on the choice */
  size = 7680;

  OLDVEC = OS.vvblkd;

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
  }

  wait_for_vblank();

  /* Load the data! */
  if (sample == SAMPLE_COLORBARS) {
    render_colorbars();
  } else {
    if (pick_day == 0) {
      snprintf(url, sizeof(url), "%s?mode=%s&sample=%d", baseurl, modes[choice], sample);
    } else {
      snprintf(url, sizeof(url), "%s?mode=%s&sample=%d&date=%02d%02d%02d", baseurl, modes[choice], sample, pick_yr, pick_mo, pick_day);
    }

    nopen(1 /* unit 1 */, url, 4 /* read */);
    /* FIXME: Check for error */

    if (size == 7680) {
      /* Single screen image to load */
      nread(1, scr_mem1, (unsigned short) size);
    } else {
      /* Multiple screen images to load... */

      OS.color4 = rgb_table[0];
      wait_for_vblank();

      for(data_read = 0; data_read < 7680; data_read += data_len)
      {
        nstatus(1);
        data_len=(OS.dvstat[1] << 8) + OS.dvstat[0];
        if (data_len == 0) break;
        if (data_read + data_len > 7680) data_len = 7680 - data_read;
        nread(1, scr_mem1 + data_read, data_len);
      }

      OS.sdmctl = 0;
      wait_for_vblank();
      OS.sdlst = dlist2;
      OS.color4 = rgb_table[1];
      wait_for_vblank();
      OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH;
      wait_for_vblank();

      for(data_read = 0; data_read < 7680; data_read += data_len)
      {
        nstatus(1);
        data_len=(OS.dvstat[1] << 8) + OS.dvstat[0];
        if (data_len == 0) break;
        if (data_read + data_len > 7680) data_len = 7680 - data_read;
        nread(1, scr_mem2 + data_read, data_len);
      }

      OS.sdmctl = 0;
      wait_for_vblank();
      OS.sdlst = dlist3;
      OS.color4 = rgb_table[2];
      wait_for_vblank();
      OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH;
      wait_for_vblank();

      for(data_read = 0; data_read < 7680; data_read += data_len)
      {
        nstatus(1);
        data_len=(OS.dvstat[1] << 8) + OS.dvstat[0];
        if (data_len == 0) break;
        if (data_read + data_len > 7680) data_len = 7680 - data_read;
        nread(1, scr_mem3 + data_read, data_len);
      }

      OS.sdlst = dlist1;
    }

    if (choice == CHOICE_MEDRES_COLOR) {
      nread(1, (char *) 712, 1);
      nread(1, (char *) 708, 3);
    }

    nclose(1 /* unit 1 */);
  }

  if (choice == CHOICE_LOWRES_RGB) {
    rgb_ctr = 0;
    dlist_hi = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) >> 8);
    dlist_lo = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) & 255);
    mySETVBV((void *) VBLANKD9);
    dli_init(dli9);
  } else if (choice == CHOICE_MEDRES_RGB) {
    // mySETVBV((void *) VBLANKD); /* FIXME */
    // dli_init(dli); /* FIXME */
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

  if (choice == CHOICE_LOWRES_RGB ||
      choice == CHOICE_MEDRES_RGB) {
    dli_clear();
    mySETVBV((void *) OLDVEC);
  }
  OS.gprior = 0;
}
