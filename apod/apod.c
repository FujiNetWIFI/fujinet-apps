/*
  apod.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  See the APOD web app (server) for additional details
  (https://github.com/FujiNetWIFI/servers/tree/main/apod).

  This is the core program ("main()"), which is repeats this loop forever:
   1. displays the menu
   2. handles the choice (downloads an image or renders a built-in (offline) 
      test pattern)
   3. displays the image

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-06-05
*/

#include <stdio.h>
#include <atari.h>
#include <string.h>

#include "globals.h"

#include "app_key.h"
#include "get_time.h"
#include "menu.h"
#include "rgb.h"
#include "screen_helpers.h"
#include "view.h"


/* The URL arguments to send to the web app via "?mode="
   for the various viewing mode choices: */
char * modes[NUM_CHOICES] = {
  "",
  "8",
  "15",
  "15dli",
  "rgb15",
  "9",
  "rgb9",
  "apac"
};

unsigned char time_buf[6];
unsigned char cur_yr, cur_mo, cur_day, pick_yr, pick_mo, pick_day;


/* The program! */
void main(void) {
  unsigned char choice, sample;

  set_screen_and_dlist_pointers();

  /* Set the defaults for the RGB table */
  set_rgb_defaults();

  /* Get the current date/time, if we can */
  get_time();

  /* Try to get our settings */
  read_settings();

  sample = SAMPLE_NONE;

  do {
    draw_menu(sample, pick_yr, pick_mo, pick_day, (cur_yr != 99));
    handle_menu(&choice, &sample);
    view(choice, sample, pick_yr, pick_mo, pick_day);
  } while(1);
}

