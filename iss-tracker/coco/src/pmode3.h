/**
 * @brief   PMODE 3 Graphic helper Functions
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Plot 2bpp pixels and print text functions.
 */

#ifndef PMODE3_H
#define PMODE3_H

#include <coco.h>

/**
 * @brief XOR all screen pixels, inverting them 
 */
void pmode3_xor(void);

/**
 * @brief copy whole screen bitmap to screen buffer
 * @param b pointer to 6144 byte bitmap
 */
void pmode3_memcpy(const byte *b);

/**
 * @brief Place pixel on PMODE3 display at x,y with color c 
 * @param x Horizontal Position (0-127) 
 * @param y Vertical Position (0-191)
 * @param c Color (0-3)
 */
void pset(int x, int y,unsigned char c);

/**
 * @brief XOR pixel on PMODE3 display at x,y 
 * @param x Horizontal Position (0-127) 
 * @param y Vertical Position (0-191)
 * @param c Color (0-3)
 * @verbose borrows fourth entry from orTable which inverts both bits.
 */
void pxor(int x, int y);

/**
 * @brief clear from lines 160-192 
 */
void clear_bottom(void);

/**
 * @brief place character from font at x,y with color
 * @param x Horizontal position (0-127) 
 * @param y Vertical position (0-191)
 * @param color to use (0-3)
 */
void putc(int x, int y, char ch, char color);

/**
 * @brief print s string at x,y with color 
 * @param x Horizontal Position (0-127)
 * @param y Vertical Position (0-191)
 * @param color to use (0-3)
 * @param s NULL terminated string to output. 
*/
void puts(int x, int y, char color, const char *s);

/**
 * @brief Set and display PMODE3 (128x192x4c)
 */
void pmode3(void);

#endif /* PMODE3_H */
