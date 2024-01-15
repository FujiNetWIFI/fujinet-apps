/**
 * @brief FujiNet weather for CoCo
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md, for details.
 * @verbose Graphics primitives for PMODE3 (128x192x2bpp)
 */

#ifndef GFX_H
#define GFX_H

#define WHITE  0x00
#define CYAN   0x01
#define PURPLE 0x02
#define ORANGE 0x03

/**
 * @brief set pixel at x,y to color c
 * @param x horizontal position (0-127)
 * @param y vertical position (0-191)
 * @param c Color (0-3)
 */
void pset(int x, int y,unsigned char c);

/**
 * @brief Put character ch in font at x,y with color c
 * @param x horizontal position (0-127)
 * @param y vertical position (0-191)
 * @param c Color (0-3)
 * @param ch Character to put (0-127)
 */
void putc(int x, int y, char c, char ch);

/**
 * @brief Put character ch in font at x,y with color c, double height.
 * @param x horizontal position (0-127)
 * @param y vertical position (0-191)
 * @param c Color (0-3)
 * @param ch Character to put (0-127)
 */
void putc_dbl(int x, int y, char c, char ch);

/**
 * @brief Put string s using putc at x,y with color c
 * @param x horizontal position (0-127)
 * @param y vertical position (0-191)
 * @param c Color (0-3)
 * @param s NULL terminated string to place on graphics screen
 */
void puts(int x, int y, char c, const char *s);

/**
 * @brief Put string s using putc at x,y with color c, double height.
 * @param x horizontal position (0-127)
 * @param y vertical position (0-191)
 * @param c Color (0-3)
 * @param s NULL terminated string to place on graphics screen
 */
void puts_dbl(int x, int y, char c, const char *s);

/**
 * @brief place icon on 8x8 boundary
 * @param x horizontal position (0-127) // must be divisible by 8
 * @param y vertical position (0-191)
 */
void put_icon(int x, int y, byte *icon);

/**
 * @brief set screen buffer address to BASIC location
 */
void set_screenbuffer(void);

/**
 * @brief set up graphics for PMODE 3
 * @param c color set to use
 */
void gfx(unsigned char c);

/**
 * @brief Clear display
 * @param c Color to clear display with
 */
void gfx_cls(unsigned char c);

#endif /* GFX_H */
