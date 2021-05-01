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
  2021-03-27 - 2021-05-01
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
#include "vblanks.h"
#include "view.h"

/* A block of space to store the graphics & display lists */
extern unsigned char scr_mem[];


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
    setup_rgb_table9();
    dlist_setup_rgb(DL_DLI(DL_GRAPHICS8), 0); /* Every line a DLI */
    OS.gprior = 64;
  } else if (choice == CHOICE_MEDRES_RGB) {
    size = 7680 * 3;
    setup_rgb_table15();
    dlist_setup_rgb(DL_GRAPHICS15, 1); /* Occasional DLI */
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
  rgb_ctr = 0;
  if (choice == CHOICE_LOWRES_RGB) {
    mySETVBV((void *) VBLANKD9);
    dli_init(dli9);
    interrupts_used = 1;
  } else if (choice == CHOICE_MEDRES_RGB) {
    mySETVBV((void *) VBLANKD15);
    dli_init(dli15);
    interrupts_used = 1;
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
      /* [Esc] Done viewing; return to main menu */
      done = 1;
    } else if ((k & 0x3F) == KEY_R || (k & 0x3F) == KEY_G || (k & 0x3F) == KEY_B || k == KEY_X) {
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
