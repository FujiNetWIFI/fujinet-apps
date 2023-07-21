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

#include <stdio.h>

extern State state;
extern unsigned char frame_counter, frame_delay;

SourceState source_state;

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
 * @brief show source message
 */
void source_show_msg(void)
{
  msg(msg_source_move);
  source_state=SOURCE_CURSOR;
}

/**
 * @brief get source piece via cursor
 */
void source_cursor(void)
{
  cursor();

  if (stick_trigger)
    if (!source_valid())
      source_state=SOURCE_INVALID_MOVE;
    else
      source_state=SOURCE_DONE;
}

/**
 * @brief invalid move made
 */
void source_invalid_move(void)
{
  if (!frame_delay)
    msg(msg_cant_do_that);

  frame_delay--;
  frame_delay&=0x3f; // 64 frames

  if (!frame_delay)
    source_state=SOURCE_SHOW_MSG;
}

/**
 * @brief Done with source, move state to destination
 */
void source_done(void)
{
  source_x = cursor_x;
  source_y = cursor_y;
  state=DESTINATION;
  source_state=SOURCE_SHOW_MSG;
}

/**
 * @brief select source square
 */
void source(void)
{
  switch(source_state)
    {
    case SOURCE_SHOW_MSG:
      source_show_msg();
      break;
    case SOURCE_CURSOR:
      source_cursor();
      break;
    case SOURCE_INVALID_MOVE:
      source_invalid_move();
      break;
    case SOURCE_DONE:
      source_done();
      break;
    }
}
