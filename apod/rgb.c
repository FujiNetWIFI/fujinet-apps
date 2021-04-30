/*
  rgb.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-04-30
*/

#include <atari.h>
#include "rgb.h"

/* In ColorView mode, we will have 3 display lists that
   we cycle through, each interleaving between three
   versions of the image (red, green, blue) */
extern unsigned char rgb_table[];


/**
 * Set up a color table of repeating Red, Green, and Blue hues
 * for ColorView 9
 */
void setup_rgb_table9(void) {
  unsigned char *rgb_ptr;
  unsigned char i;

  rgb_ptr = rgb_table;
  for(i = 0; i < 64; i++) {
    *rgb_ptr++ = rgb_red;
    *rgb_ptr++ = rgb_grn;
    *rgb_ptr++ = rgb_blu;
  }
}

/**
 * Set up a color table of repeating shades of Red, Green, and Blue hues
 * for ColorView 15
 */
void setup_rgb_table15(void) {
  unsigned char *rgb_ptr;
  unsigned char i;

  rgb_ptr = rgb_table;
  for(i = 0; i < 28; i++) {
    *rgb_ptr++ = (rgb_red + 4);
    *rgb_ptr++ = (rgb_red + 8);
    *rgb_ptr++ = (rgb_red + 14);

    *rgb_ptr++ = (rgb_grn + 4);
    *rgb_ptr++ = (rgb_grn + 8);
    *rgb_ptr++ = (rgb_grn + 14);

    *rgb_ptr++ = (rgb_blu + 4);
    *rgb_ptr++ = (rgb_blu + 8);
    *rgb_ptr++ = (rgb_blu + 14);
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
    set_rgb_defaults();
  }
}

/**
 * Set the default hues for the RGB table
 */
void set_rgb_defaults(void) {
  rgb_red = DEFAULT_RGB_RED;
  rgb_grn = DEFAULT_RGB_GRN;
  rgb_blu = DEFAULT_RGB_BLU;
  apac_lum = DEFAULT_APAC_LUM;
}
