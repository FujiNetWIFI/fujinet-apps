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

typedef unsigned short Position; 

#define DISC_CHAR_1 0x3B
#define DISC_CHAR_2 0x3C
#define DISC_CHAR_3 0x3D
#define DISC_CHAR_4 0x3E

#define START_POS_1 27
#define START_POS_2 28
#define START_POS_3 35
#define START_POS_4 36

#define FOOTER_OFFSET 484

/* bias value to convert Disc value to color ram value */
#define COLOR_RAM_BIAS 7

#endif /* BOARD_H */
