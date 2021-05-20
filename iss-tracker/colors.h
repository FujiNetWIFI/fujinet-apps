#ifndef COLORS_H
#define COLORS_H

#define DEFAULT_COLOR4 0x8A /* blue oceans */
#define DEFAULT_COLOR0 0x88 /* grid over oceans */
#define DEFAULT_COLOR1 0xB4 /* continents (also, lum used by text) */
#define DEFAULT_COLOR2 0xB0 /* grid over continents (also, lum used by text background) */

extern unsigned char color0, color1, color2, color4;

void set_default_colors(void);

#endif // COLORS_H

