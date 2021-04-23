/*
  rgb.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-23
*/

#include <atari.h>
#include "rgb.h"

/* In ColorView mode, we will have 3 display lists that
   we cycle through, each interleaving between three
   versions of the image (red, green, blue) */
extern unsigned char rgb_table[];


/**
 * Set up a color table of repeating Red, Green, and Blue hues
 */
void setup_rgb_table(void) {
  unsigned char *rgb_ptr;
  unsigned char i;

  rgb_ptr = rgb_table;
  for(i = 0; i < 64; i++)
  {
    *rgb_ptr++ = rgb_red;
    *rgb_ptr++ = rgb_grn;
    *rgb_ptr++ = rgb_blu;
  }
}


void handle_rgb_keypress(unsigned char k) {
  if (k == KEY_R) {
    rgb_red += 16;
  } else if (k == KEY_G) {
    rgb_grn += 16;
  } else if (k == KEY_B) {
    rgb_blu += 16;
  } else if (k == (KEY_R | KEY_SHIFT)) {
    rgb_red -= 16;
  } else if (k == (KEY_G | KEY_SHIFT)) {
    rgb_grn -= 16;
  } else if (k == (KEY_B | KEY_SHIFT)) {
    rgb_blu -= 16;
  } else if (k == KEY_X) {
    rgb_red = DEFAULT_RGB_RED;
    rgb_grn = DEFAULT_RGB_GRN;
    rgb_blu = DEFAULT_RGB_BLU;
  }
}

/**
 * Set the default hues for the RGB table
 */
void set_rgb_defaults(void) {
  rgb_red = DEFAULT_RGB_RED;
  rgb_grn = DEFAULT_RGB_GRN;
  rgb_blu = DEFAULT_RGB_BLU;
}
