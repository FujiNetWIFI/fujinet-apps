/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Type definitions
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdbool.h>

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
 * @brief Source state
 */
typedef enum _sourceState
  {
    SOURCE_SHOW_MSG,
    SOURCE_CURSOR,
    SOURCE_INVALID_MOVE,
    SOURCE_DONE,
  }
  SourceState;

/**
 * @brief Destination state
 */
typedef enum _destinationState
  {
    DESTINATION_SHOW_MSG,
    DESTINATION_CURSOR,
    DESTINATION_BACK_OUT,
    DESTINATION_INVALID_MOVE,
    DESTINATION_DONE,
  }
  DestinationState;

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

/**
 * @brief stick directions
 */
typedef enum _direction
  {
    RIGHT = 7,
    LEFT  = 11,
    DOWN  = 13,
    UP    = 14
  } Direction;

/**
 * @brief stick trigger state 0=pushed
 */
typedef bool Trigger;

#endif /* TYPEDEFS_H */
