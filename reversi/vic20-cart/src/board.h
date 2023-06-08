/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef BOARD_H
#define BOARD_H

typedef enum _disc
  {
    NONE,
    BLACK,
    WHITE
  } Disc;

/**
 * @brief a board position scanned left right, top bottom, 0-63
 */
typedef unsigned short Position; 

/**
 * @brief store returned valid moves
 * @verbose bit 0 = left
 *          bit 1 = right
 *          bit 2 = down
 *          bit 3 = up
 *          bit 4 = down-left
 *          bit 5 = down-right
 *          bit 6 = up-left
 *          bit 7 = up-right
 *          no bits = no valid moves
 */
typedef unsigned char ValidMoves;

/* symmetrical board size in rows/columns */
#define BOARD_SIZE 8

#define DISC_CHAR_1 0x3B
#define DISC_CHAR_2 0x3C
#define DISC_CHAR_3 0x3D
#define DISC_CHAR_4 0x3E

#define START_POS_1 27
#define START_POS_2 28
#define START_POS_3 35
#define START_POS_4 36

#define FOOTER_OFFSET 484

/* Color RAM value for empty pieces */
#define COLOR_NONE 0x0D

/* bias value to convert Disc value to color ram value */
#define COLOR_RAM_BIAS 7

#define SCREEN_SIZE_X 22
#define SCREEN_SIZE_Y 23

#endif /* BOARD_H */
