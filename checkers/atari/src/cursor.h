/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Cursor routines
 */

#ifndef CURSOR_H
#define CURSOR_H

#include "typedefs.h"

/**
 * @brief current horizontal cursor position (0-3)
 */
extern unsigned char cursor_x;

/**
 * @brief current vertical cursor position (0-7)
 */
extern unsigned char cursor_y;

/**
 * @brief Move and position cursor, exit on trigger
 */
void cursor(void);

/**
 * @brief get piece at cursor
 * @return piece at cursor (see typedefs.h)
 */
Piece cursor_get(void);

#endif /* CURSOR_H */
