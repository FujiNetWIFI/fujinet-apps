/*
  menu.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-24
*/

#include <atari.h>
#include <stdio.h>
#include <string.h>
#include "version.h"
#include "get_time.h"
#include "menu.h"
#include "screen_helpers.h"
#include "myprint.h"
#include "rgb.h"

unsigned char choice_keys[NUM_CHOICES] = {
  KEY_NONE,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F
};

unsigned char sample_keys[NUM_SAMPLES] = {
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_9
};

extern unsigned char scr_mem[];

/**
 * Set up display list for title/menu screen
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

  dlist1[18] = DL_BLK2;
  dlist1[19] = DL_GRAPHICS2;

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

/**
 * Display the chosen date (or "current") on the menu
 */
void show_chosen_date(unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly) {
  char str[20];

  myprint(scr_mem, 2, 15, "                  ");
  if (d != 0) {
    sprintf(str, "20%02d-%02d-%02d", y, m, d);
    myprint(scr_mem, 2, 15, str);
    myprint(scr_mem, 13, 15, "[CT/SH]");
  } else {
    if (!loaded_properly) {
      myprint(scr_mem, 2, 15, "[CTRL-T] get time");
    } else {
      myprint(scr_mem, 2, 15, "current");
    }
  }
}

/**
 * Display sample choice, if any, on the menu
 */
void show_sample_choice(char sample) {
  char tmp_str[2];

  myprint(scr_mem, 18, 17, "  ");
  if (sample == SAMPLE_COLORBARS) {
    myprint(scr_mem, 18, 17, "CB");
  } else if (sample) {
    tmp_str[0] = sample + '0';
    tmp_str[1] = '\0';
    myprint(scr_mem, 19, 17, tmp_str);
  }
}

void draw_menu(char sample, unsigned char y, unsigned char m, unsigned char d, unsigned char loaded_properly, unsigned char rgb_red, unsigned char rgb_grn, unsigned char rgb_blu) {
  char str[20];

  dlist_setup_menu();

                /*--------------------*/
  myprint(scr_mem, 1,  0, "#FUJINET Astronomy");
  myprint(scr_mem, 1,  1, "Picture Of the Day");

                /*--------------------*/
  myprint(scr_mem, 1,  2, "bill kendrick 2021");
  myprint(scr_mem, 0,  3, "with help from apc");
  myprint(scr_mem, 10 - strlen(VERSION) / 2, 4, VERSION);

                /*--------------------*/
  myprint(scr_mem, 0,  5, "________HOW_________");
  myprint(scr_mem, 0,  6, "[A] hi-res mono");
  myprint(scr_mem, 0,  7, "[B] med-res 4 color");
  myprint(scr_mem, 0,  8, "[C] lo-res 16 shade");
  myprint(scr_mem, 0,  9, "[D]*lo-res 4K color");
//  myprint(scr_mem, 0, 10, "[E]*med-res 64 color");
  myprint(scr_mem, 0, 11, "[F]*lo-res 256 color");

                /*--------------------*/
  myprint(scr_mem, 0, 12, "________WHAT________");
  myprint(scr_mem, 0, 13, "[0] get apod");
  myprint(scr_mem, 0, 14, "[<=>] change date");
  show_chosen_date(y, m, d, loaded_properly);
  myprint(scr_mem, 0, 16, "[1-5] get samples");
  myprint(scr_mem, 0, 17, "[9] color bars");
  show_sample_choice(sample);

                /*--------------------*/
  myprint(scr_mem, 0, 18, "___*WHILE_VIEWING___");
  sprintf(str, "[R]=%02d [G]=%02d [B]=%02d", rgb_red >> 4, rgb_grn >> 4, rgb_blu >> 4);
  myprint(scr_mem, 0, 19, str);
  sprintf(str, "[L]=%02d  [X] defaults", apac_lum);
  myprint(scr_mem, 0, 20, str);
  myprint(scr_mem, 0, 21, "[ESC] return to menu");
}

/**
 * Pick the current date (as fetched from #FujiNet's APETIME)
 */
void pick_today() {
  pick_yr = cur_yr;
  pick_mo = cur_mo;
  pick_day = cur_day;
}


void handle_menu(unsigned char * choice, char * sample) {
  unsigned char keypress, date_chg;
  int i;

  /* Accept a choice */
  *choice = CHOICE_NONE;
  OS.ch = KEY_NONE;
  do {
    while (OS.ch == KEY_NONE) { }
    keypress = OS.ch;
    OS.ch = KEY_NONE;

    for (i = 0; i < NUM_CHOICES; i++) {
      if (choice_keys[i] == keypress) {
        *choice = i;
      }
    }

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
  } while (*choice == CHOICE_NONE);
}
