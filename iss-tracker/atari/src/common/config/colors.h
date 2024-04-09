#ifndef COLORS_H
#define COLORS_H

#define DEFAULT_COLOR4 0x00 /* border & tracking dots */
#define DEFAULT_COLOR0 0x8A /* oceans */
#define DEFAULT_COLOR1 0x88 /* grid over oceans */
#define DEFAULT_COLOR2 0xB4 /* continents */

extern unsigned char color0, color1, color2, color4;

void set_default_colors(void);

#endif // COLORS_H

