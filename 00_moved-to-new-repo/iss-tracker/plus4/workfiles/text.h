#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

/**
 * @brief Quick and Dirty text routines for C64 bitmap mode
 * @author Thomas Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

/**
 * @brief place string s at cursor
 * @param s The string of PETSCII characters to type
 */
void text(const char *s, bool i);

/**
 * @brief place cursor at x,y
 * @param x the horizontal offset of line (0-39)
 * @param y the line (0-24)
 */
void graphics_gotoxy(unsigned char x, unsigned char y);

/**
 * @brief clear text window 
 */
void text_clear(void);

#endif /* TEXT_H */
