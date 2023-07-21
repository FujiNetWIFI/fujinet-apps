/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Source routines
 */

#include <stdbool.h>
#include <atari.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "source.h"
#include "msg.h"
#include "player.h"
#include "stick.h"

extern State state, previous_state, return_state;
extern unsigned char frame_counter;

/**
 * @brief selected source horizontal (0-3)
 */
unsigned char source_x;

/**
 * @brief selected source vertical (0-7)
 */
unsigned char source_y;

/**
 * @brief is source valid?
 */
bool source_valid(void)
{
  Piece p = cursor_get();
  
  if (!p)         // empty
    return false; // not valid
    
  switch (current_player)
    {
    case PLAYER_1:
      return p == RED;
    case PLAYER_2:
      return p == WHITE;
    }
}

/**
 * @brief blink selected source square
 */
void source_blink(void)
{
  /* Piece p = board_get(source_x, source_y); */

  /* board_set(source_x,source_y,NONE); */

  /* delay(SOURCE_BLINK_DELAY); */

  /* board_set(source_x,source_y,p); */

  /* delay(SOURCE_BLINK_DELAY); */
}

/**
 * @brief select source square
 */
void source(void)
{
  if (state != previous_state)
    msg(msg_source_move);
  
  cursor();

  if (stick_trigger)
    {
      if (!source_valid())
	{
	  return_state=SOURCE;
	  state=INVALID_MOVE;
	}
    }
  
  /* while (OS.strig0) */
  /*   { */
  /*     cursor(); */
  /*   } */

  /* if (!source_valid()) */
  /*   { */
  /*     msg(msg_cant_do_that); */
  /*     delay(SOURCE_MSG_DELAY); */
  /*   } */
  /* else */
  /*   { */
  /*     source_x = cursor_x; */
  /*     source_y = cursor_y; */
  /*     state=DESTINATION; */
  /*   } */
}
