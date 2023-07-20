/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Board routines
 */

#ifndef BOARD_H
#define BOARD_H

#include "typedefs.h"

/**
 * @brief return color at board position
 * @brief x horizontal position (0-3)
 * @brief y vertical position (0-7)
 * @return Piece (see typedef.h)
 */
Piece board_get(unsigned char x, unsigned char y);

/**
 * @brief set piece on board
 * @brief x horizontal position (0-3)
 * @brief y vertical position (0-7)
 * @param Piece (see typedef.h)
 */
void board_set(unsigned char x, unsigned char y, Piece piece);

/**
 * @brief Reset board piece positions
 */
void board_reset(void);

/**
 * @brief draw board
 */
void board(void);

#endif /* BOARD_H */
