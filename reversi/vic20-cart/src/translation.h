/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "board.h"

/**
 * @brief Translate Atari coordinates to board position
 * @param x X board position
 * @param y Y board position
 * @return Actual board position (0-63)
 */
Position translation(unsigned char x, unsigned char y);

#endif /* TRANSLATION_H */
