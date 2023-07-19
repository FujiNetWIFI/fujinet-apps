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
 * @param pos Board position (0-31)
 * @return Piece (see typedef.h)
 */
Piece board_get(unsigned char pos);

/**
 * @brief set piece on board
 * @param pos board position (0-31)
 * @param Piece (see typedef.h)
 */
void board_set(unsigned char pos, Piece piece);

/**
 * @brief Reset board piece positions
 */
void board_reset(void);

/**
 * @brief draw board
 */
void board(void);

#endif /* BOARD_H */
