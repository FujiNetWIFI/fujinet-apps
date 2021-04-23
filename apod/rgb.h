#ifndef RGB_H
#define RGB_H

/* Defaults that look good on my NTSC Atari 1200XL connected
   to a Commodore 1902 monitor with Tint knob at its default,
   and Color knob just below its default.

   + A good rainbow:
     https://apod.nasa.gov/apod/ap060423.html

   + Has solid blue, green, and red dots:
     https://apod.nasa.gov/apod/ap060426.html
*/

#define DEFAULT_RGB_RED 0x20 /* 2 "orange" */
#define DEFAULT_RGB_GRN 0xD0 /* 13 "yellow green" */
#define DEFAULT_RGB_BLU 0xB0 /* 11 "blue green" */

extern unsigned char rgb_table[];

extern unsigned char rgb_red, rgb_grn, rgb_blu;

void setup_rgb_table(void);
void handle_rgb_keypress(unsigned char k);
void set_rgb_defaults(void);

#endif // RGB_H

