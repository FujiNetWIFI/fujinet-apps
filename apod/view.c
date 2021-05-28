/*
  view.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module sets up an image viewing screen (see "dlists" module),
  fetches an image from the APOD server (see "fetch" module)
  or renders a test pattern (see "colorbars" module),
  displays it (invoking VBI and DLI routines, as necessary;
  see "dli*" and "vblanks" modules), and finally handles
  input from the user, returning to the main() loop when they
  are done looking at the picture.

  Keyboard input during viewing is handled here
  (directly, e.g. [L] luminance controls for "Lo-res 256 color" ("APAC")
  mode, or indirectly, e.g. [R]/[G]/[B] hues for "ColorView" modes;
  see "rgb" module).

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-05-27
*/

#include <stdio.h>
#include <atari.h>
#include "colorbars.h"
#include "dlists.h"
#include "dli15.h"
#include "dli256.h"
#include "dli9.h"
#include "fetch.h"
#include "interrupt_helpers.h"
#include "menu.h"
#include "rgb.h"
#include "screen_helpers.h"
#include "speak.h"
#include "vblanks.h"
#include "view.h"

/**
 * Fetch or render the image, and view it!
 */
void view(unsigned char choice, char sample, unsigned char pick_yr, unsigned pick_mo, unsigned pick_day) {
  int size, i;
  unsigned char k, interrupts_used;
  unsigned char * txt_buf;

  txt_buf = (char *) (txt_mem + 40);

  /* Set up the display, based on the choice */
  size = 7680;

  OLDVEC = OS.vvblkd;

  /* Prepare display for loading image... */
  screen_off();
  if (choice == CHOICE_HIRES_MONO) {
    dlist_setup(DL_GRAPHICS8, 1);
    OS.color4 = 128; /* Border (no reason) */
    OS.color2 = 0; /* Background */
    OS.color1 = 15; /* Foreground */
  } else if (choice == CHOICE_LOWRES_GREY) {
    dlist_setup(DL_GRAPHICS8, 0);
    OS.gprior = 64;
    OS.color4 = 0; /* Greyscale */
  } else if (choice == CHOICE_MEDRES_COLOR) {
    dlist_setup(DL_GRAPHICS15, 1);
    /* (Colors will be replaced by palette fetched from server) */
    OS.color4 = 0; /* Background (black) */
    OS.color0 = 4; /* Dark foreground */
    OS.color1 = 8; /* Medium foreground */
    OS.color2 = 14; /* Light foreground */
  } else if (choice == CHOICE_LOWRES_RGB) {
    size = 7680 * 3;
    setup_rgb_table9();
    dlist_setup_rgb(DL_DLI(DL_GRAPHICS8), 0); /* DLI every scanline */
    OS.gprior = 64;
  } else if (choice == CHOICE_MEDRES_RGB) {
    size = 7680 * 3;
    setup_rgb_table15();
    dlist_setup_rgb(DL_GRAPHICS15, 1); /* DLI at the top only */
  } else if (choice == CHOICE_LOWRES_256) {
    size = 7680 * 2;
    dlist_setup_apac();
  }
  screen_on();


  /* Load or render the image! */
  wait_for_vblank();
  if (sample == SAMPLE_COLORBARS) {
    render_colorbars(choice);
  } else {
    fetch_image(choice, sample, size, pick_yr, pick_mo, pick_day);
  }

  /* Screen-mem-ify the description text */
  for (i = 0; i < 40; i++) {
    if (txt_buf[i] < 32) {
      txt_mem[i] = txt_buf[i] + 64;
    } else if (txt_buf[i] < 96) {
      txt_mem[i] = txt_buf[i] - 32;
    } else {
      txt_mem[i] = txt_buf[i];
    }
  }

  dlist_hi = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) >> 8);
  dlist_lo = (unsigned char) (((unsigned int) (scr_mem + DLIST_OFFSET)) & 255);

  /* Enable interrupt-driven graphics mode, if applicable */
  rgb_ctr = 0;
  interrupts_used = 1;
  if (choice == CHOICE_LOWRES_RGB) {
    mySETVBV((void *) VBLANKD9);
    dli_init(dli9);
  } else if (choice == CHOICE_MEDRES_RGB) {
    mySETVBV((void *) VBLANKD15);
    dli_init(dli15);
  } else if (choice == CHOICE_LOWRES_256) {
    mySETVBV((void *) VBLANKD_APAC);
    dli_init(dli256);
  } else {
    interrupts_used = 0;
  }


  /* Accept keypresses while viewing */
  OS.ch = KEY_NONE;
  do {
    k = OS.ch;
    if ((k & 0x3F) == KEY_R || (k & 0x3F) == KEY_G || (k & 0x3F) == KEY_B || k == KEY_X) {
      /* [R], [G], or [B] key, with/without [Shift]: adjust hues for RGB modes
         [X]: reset RGB mode hues and APAC luminence */
      handle_rgb_keypress(k);
      if (choice == CHOICE_LOWRES_RGB) {
        setup_rgb_table9();
      } else {
        setup_rgb_table15();
      }
      OS.ch = KEY_NONE;
    } else if (k == KEY_L) {
      /* [L]: Increase APAC luminence */
      apac_lum = (apac_lum + 2) % 16;
      OS.ch = KEY_NONE;
    } else if (k == (KEY_L | KEY_SHIFT)) {
      /* [Shift]+[L]: Decrease APAC luminence */
      if (apac_lum == 0) {
        apac_lum = 14;
      } else {
        apac_lum -= 2;
      }
      OS.ch = KEY_NONE;
    } else if (k == KEY_V) {
      /* [V]: Speak the title */
      speak();
      OS.ch = KEY_NONE;
    }
  } while (k != KEY_ESC); /* [Esc] Done viewing; return to main menu */
  OS.ch = KEY_NONE;

  /* Clear interrupt and drop GTIA mode, if set */
  if (interrupts_used) {
    dli_clear();
    mySETVBV((void *) OLDVEC);
  }
  OS.gprior = 0;
}
