/*
  apod.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  See the APOD web app (server)

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-21
*/

#include <stdio.h>
#include <atari.h>
#include <string.h>

#include "globals.h"

#include "get_time.h"
#include "menu.h"
#include "rgb.h"
#include "screen_helpers.h"
#include "view.h"


/* A block of space to store the graphics & display lists */
extern unsigned char scr_mem[];

/* The URL arguments to send to the web app via "?mode="
   for the various viewing mode choices: */
char * modes[NUM_CHOICES] = {
  "",
  "8",
  "15",
  "9",
  "rgb9",
  "rgb15"
};

unsigned char last_day[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/**
 * Pick the current date (as fetched from #FujiNet's APETIME)
 */
void pick_today() {
  pick_yr = cur_yr;
  pick_mo = cur_mo;
  pick_day = cur_day;
}

/* The program!  FIXME: Split into functions! */
void main(void) {
  unsigned char keypress, choice;
  int i;
  unsigned char sample = 0, date_chg;

  set_screen_and_dlist_pointers();

  /* Set the defaults for the RGB table */
  set_rgb_defaults();

  /* Get the current date/time, if we can */
  get_time();

  do {
    /* Prompt user for the preferred viewing mode */
    draw_menu(sample, pick_yr, pick_mo, pick_day, (cur_yr != 99), rgb_red, rgb_grn, rgb_blu);

    /* Accept a choice */
    choice = CHOICE_NONE;
    OS.ch = KEY_NONE;
    do {
      while (OS.ch == KEY_NONE) { }
      keypress = OS.ch;
      OS.ch = KEY_NONE;

      for (i = 0; i < NUM_CHOICES; i++) {
        if (choice_keys[i] == keypress) {
          choice = i;
        }
      }

      for (i = 0; i < NUM_SAMPLES; i++) {
        if (sample_keys[i] == keypress) {
          sample = i;
          show_sample_choice(sample);
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
    } while (choice == CHOICE_NONE);

    view(choice, sample, pick_yr, pick_mo, pick_day);
  } while(1);
}
