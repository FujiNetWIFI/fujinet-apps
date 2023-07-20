/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Type definitions
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/**
 * @brief Game state
 */
typedef enum _state
  {
    BOARD,
    SOURCE,
    DESTINATION,
    CHECK,
    MOVE,
  }
  State;

/**
 * @brief a piece on the board
 */
typedef enum _piece
  {
    NONE = 0,
    RED = 1,
    RED_KING = 2,
    CURSOR = 0x43,
    WHITE = 0x81,
    WHITE_KING = 0x82,
  }
  Piece;

/**
 * @brief player
 */
typedef enum _player
  {
    PLAYER_1,
    PLAYER_2,
  }
  Player;

/**
 * @brief Board Position
 */
typedef signed char Position;

#endif /* TYPEDEFS_H */
