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
#include "delay.h"

#define SOURCE_MSG_DELAY 30 /* roughly half a second. */

extern State state;

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
 * @brief select source square
 */
void source(void)
{
  msg(msg_source_move);

  while (OS.strig0)
    {
      cursor();
    }

  if (!source_valid())
    {
      msg(msg_cant_do_that);
      delay(SOURCE_MSG_DELAY);
    }
  
    
}
