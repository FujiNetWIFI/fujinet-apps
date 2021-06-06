/*
  menu.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module draws the main menu, and handles the
  input from the main menu.

  If the user has chosen to view an image, we'll return
  to the main loop, where the "view" module will take
  over (fetch/render and display the image, and handle
  input during the viewing process).

  Options which do not leave the main menu (changing the
  APOD photo date, picking a sample, re-requesting the
  current time from the #FujiNet, etc.), are all handled
  here.

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-06-05
*/

#include <atari.h>
#include <stdio.h>
#include <string.h>
#include "version.h"
#include "app_key.h"
#include "get_time.h"
#include "menu.h"
#include "screen_helpers.h"
#include "myprint.h"
#include "rgb.h"

/* Which keypresses correspond to viewing mode
 * choices (see "menu.h").  Once pressed, we'll
 * leave the main menu, and let the "view" module
 * do its work!
 */
unsigned char choice_keys[NUM_CHOICES] = {
  KEY_NONE,
  KEY_H, /* High-res mono */
  KEY_M, /* Med-res 4-color */
  KEY_Z, /* Med-res 4-color-per-scanline [FIXME meaningless abbreviation] */
  KEY_C, /* Colorful/ColorView */
  KEY_G, /* Greyscale [FIXME conflict!] */
  KEY_F, /* Four-thousand ninety-six */
  KEY_A  /* APAC */
  /* Other keys used here:
  0,1,2,3,5
  <,=,>
  R,G,B,L,X
  S
  */
};

/* Which keypresses correspond to sample choices
 * (see "menu.h").  When pressed, we'll update the
 * choice, show it on the screen, and remain on the
 * main menu
 */
unsigned char sample_keys[NUM_SAMPLES] = {
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5
};

/* Note: Other key presses and key combinations
 * ("KEY_x" values) are utilized by our "handle_menu()"
 * function; see them there.
 */

/**
 * Set up display list for title/menu screen
 * FIXME: Move to dlists.c/.h? -bjk 2021.05.01
 */
void dlist_setup_menu(void) {
  int dl_idx;

  screen_off();

  dlist1[0] = DL_BLK1;
  dlist1[1] = DL_BLK1;
  dlist1[2] = DL_BLK8;

  dlist1[3] = DL_LMS(DL_GRAPHICS2);
  dlist1[4] = ((unsigned int) scr_mem) & 255;
  dlist1[5] = ((unsigned int) scr_mem) >> 8;
  dlist1[6] = DL_GRAPHICS2;

  dlist1[7] = DL_BLK2;
  dlist1[8] = DL_GRAPHICS1;
  dlist1[9] = DL_GRAPHICS0;

  dlist1[10] = DL_BLK2;
  dlist1[11] = DL_GRAPHICS2;

  for (dl_idx = 12; dl_idx < 35; dl_idx++) {
    dlist1[dl_idx] = DL_GRAPHICS1;
  }

  dlist1[19] = DL_BLK2;
  dlist1[20] = DL_GRAPHICS2;

  dlist1[25] = DL_BLK2;
  dlist1[26] = DL_GRAPHICS2;

  dlist1[36] = DL_JVB;
  dlist1[37] = ((unsigned int) dlist1) & 255;
  dlist1[38] = ((unsigned int) dlist1) >> 8;

  OS.color4 = 0x00;
  OS.color0 = 0x8F;
  OS.color1 = 0x0A;
  OS.color2 = 0x80;
  OS.color3 = 0x88;

  screen_on();
}

#ifndef APODTEST
/**
 * Display the chosen date (or "current") on the menu
 */
void show_chosen_date(unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly) {
  char str[20];

  memset(scr_mem + (16 * 20), 0, 20);
  if (d != 0) {
    sprintf(str, "20%02d-%02d-%02d", y, m, d);
    myprint(2, 16, str);
    myprint(13, 16, "[CT/SH]");
  } else {
    if (!loaded_properly) {
      myprint(2, 16, "[CTRL-T] get time");
    } else {
      myprint(2, 16, "current");
    }
  }
}

/**
 * Display sample choice, if any, on the menu
 */
void show_sample_choice(char sample) {
  memset(scr_mem + 13 * 20 + 16, 0, 4);
  if (sample) {
    scr_mem[13 * 20 + 19] = sample + 16;
  } else {
    myprint(16, 13, "APOD");
  }
}
#endif

void show_settings(void) {
  char str[20];

  sprintf(str, "[R]=%02d [G]=%02d [B]=%02d", rgb_red >> 4, rgb_grn >> 4, rgb_blu >> 4);
  myprint(0, 19, str);
  sprintf(str, "[L]=%02d  [X] defaults", apac_lum);
  myprint(0, 20, str);
}

void show_version(unsigned char nickname) {
  memset(scr_mem + 80, 0, 20);
  if (nickname) {
    myprint(10 - strlen(VERSION_NICKNAME) / 2, 4, VERSION_NICKNAME);
  } else {
    myprint(10 - strlen(VERSION) / 2, 4, VERSION);
  }
}

/**
 * Draw the full menu, with the current settings
 * already displayed.
 */
void draw_menu(char sample, unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly) {
  dlist_setup_menu();

  myprint(1,  0, "#FUJINET Astronomy");
  myprint(1,  1, "Picture Of the Day");

  myprint(2,  2, "by bill kendrick");
  myprint(0,  3, "with help from apc");
  show_version(0);

  myprint(8,  5, "HOW");
  myprint(0,  6, "[H] hi-res mono");
  myprint(0,  7, "[M] med-res 4 color");
  myprint(0,  8, "[Z] med-res 4/scanln");
  myprint(0,  9, "[C]*med-res 64 color");
  myprint(0, 10, "[G] lo-res 16 shade");
  myprint(0, 11, "[F]*lo-res 4K color");
  myprint(0, 12, "[A]*lo-res 256 color");

  myprint(8, 13, "WHAT");
#ifdef APODTEST
  myprint(0, 14, "color bars");
#else
  myprint(0, 14, "[0] get apod");
  myprint(0, 15, "[<=>] change date");
  show_chosen_date(y, m, d, loaded_properly);
  myprint(0, 17, "[1-5] get samples");
  show_sample_choice(sample);
#endif

  myprint(2, 18, "*ADJUST_SETTINGS");
  show_settings();
  myprint(0, 21, "[S] save settings");
}

/**
 * Pick the current date (as fetched from #FujiNet's APETIME)
 */
void pick_today() {
  pick_yr = cur_yr;
  pick_mo = cur_mo;
  pick_day = cur_day;
}


/**
 * Handle main menu keyboard input from the user
 *
 * @param byte* choice -- the display choice (returned to the main()
 *   loop, so it can be passed along to the "view" module")
 * @param byte* sample -- the sample choice (returned, as above)
 *
 * NOTE: Some other settings (the APOD date to fetch) are
 * handled via globals.
 */
void handle_menu(unsigned char * choice, char * sample) {
  unsigned char keypress;
#ifndef APODTEST
  unsigned char date_chg;
#endif
  unsigned char i;

  /* Accept a choice */
  *choice = CHOICE_NONE;
  OS.ch = KEY_NONE;
  do {
    while (OS.ch == KEY_NONE) {
      if (OS.rtclok[2] == 0) {
        show_version((OS.rtclok[1] & 1));
      }
    }
    keypress = OS.ch;
    OS.ch = KEY_NONE;

    for (i = 0; i < NUM_CHOICES; i++) {
      if (choice_keys[i] == keypress) {
        *choice = i;
      }
    }

#ifndef APODTEST
    for (i = 0; i < NUM_SAMPLES; i++) {
      if (sample_keys[i] == keypress) {
        *sample = i;
        show_sample_choice(*sample);
      }
    }

    date_chg = 0;
    if (keypress == KEY_LESSTHAN) {
      /* [<] - Prev day */
      if (pick_day == 0) {
        pick_today();
      }
      if (pick_day > 1) {
        pick_day--;
      } else {
        if (pick_mo > 1) {
          pick_mo--;
        } else {
          pick_yr--;
          pick_mo = 12;
        }
        pick_day = last_day[pick_mo];
      }
      date_chg = 1;
    } else if (keypress == (KEY_LESSTHAN | KEY_SHIFT)) {
      /* [Shift]+[<] - Prev month */
      if (pick_mo > 1) {
        pick_mo--;
      } else {
        pick_yr--;
        pick_mo = 12;
      }
      date_chg = 1;
    } else if (keypress == (KEY_LESSTHAN | KEY_CTRL)) {
      /* [Ctrl]+[<] - Prev year */
      pick_yr--;
      date_chg = 1;
    } else if (keypress == KEY_GREATERTHAN) {
      /* [>] - Next day */
      if (pick_day == 0) {
        pick_today();
      }
      if (pick_day < last_day[pick_mo]) {
        pick_day++;
      } else {
        pick_day = 1;
        if (pick_mo < 12) {
          pick_mo++;
        } else {
          pick_mo = 1;
          pick_yr++;
        }
      }
      date_chg = 1;
    } else if (keypress == (KEY_GREATERTHAN | KEY_SHIFT)) {
      /* [Shift]+[>] - Next month */
      if (pick_mo < 12) {
        pick_mo++;
      } else {
        pick_mo = 1;
        pick_yr++;
      }
      date_chg = 1;
    } else if (keypress == (KEY_GREATERTHAN | KEY_CTRL)) {
      /* [Ctrl]+[>] - Next year */
      pick_yr++;
      date_chg = 1;
    } else if (keypress == KEY_EQUALS) {
      /* [=] - Choose current day (server-side) */
      pick_day = 0;
      date_chg = 1;
    } else if (keypress == (KEY_T | KEY_CTRL)) {
      /* [Ctrl]+[T] - Try to fetch time from #FujiNet APETIME again */
      get_time();
      pick_yr = cur_yr;
      pick_mo = cur_mo;
      pick_day = cur_day;
      date_chg = 1;
    }
#endif
    if (keypress == KEY_X ||
      (keypress & 0x3F) == KEY_R || (keypress & 0x3F) == KEY_G || (keypress & 0x3F) == KEY_B ||
      (keypress & 0x3F) == KEY_L
    ) {
      /* ([Shift]) [R], [G], [B] adjust ColorView hue,
         ([Shift]) [L] adjust APAC luminence,
         [X] reset settings */
      if (keypress == KEY_L) {
        apac_lum = (apac_lum + 2) % 16;
      } else if (keypress == (KEY_SHIFT | KEY_L)) {
        apac_lum = (apac_lum - 2) % 16;
      } else {
        handle_rgb_keypress(keypress);
      }
      show_settings();
    } else if (keypress == KEY_S) {
      /* [S] Save settings */
      myprint(18, 21, "..");
      if (write_settings()) {
        myprint(18, 21, "OK");
      } else {
        myprint(18, 21, ":(");
      }
    }

#ifndef APODTEST
    /* Date changed! Keep it sane, and display it */
    if (date_chg) {
      if (pick_day > last_day[pick_mo]) {
        pick_day = last_day[pick_mo];
      }
      if (pick_yr > cur_yr ||
          (pick_yr == cur_yr && pick_mo > cur_mo) ||
          (pick_yr == cur_yr && pick_mo == cur_mo && pick_day > cur_day)) {
        pick_today();
      }
      show_chosen_date(pick_yr, pick_mo, pick_day, (cur_yr != 99));
    }
#endif
  } while (*choice == CHOICE_NONE);
}
