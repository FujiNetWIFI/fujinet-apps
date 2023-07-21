/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Destination routines
 */

#include <atari.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "destination.h"
#include "source.h"
#include "msg.h"
#include "stick.h"

extern State state;
extern unsigned char frame_counter, frame_delay;

/**
 * @brief the current destination state
 */
DestinationState destination_state;

/**
 * @brief selected destination horizontal (0-3)
 */
unsigned char destination_x;

/**
 * @brief selected destination vertical (0-7)
 */
unsigned char destination_y;

/**
 * @brief the selected destination position
 */
Piece destination_piece;

/**
 * @brief show destination message
 */
void destination_show_msg(void)
{
  msg(msg_destination_move);
  destination_state=DESTINATION_CURSOR;
}

/**
 * @brief is destination valid?
 * @return true if cursor is on a valid destination
 */
bool destination_valid(void)
{
  return false; // for now.
}

/**
 * @brief get destination piece via cursor
 */
void destination_cursor(void)
{
  cursor();
  source_blink();

  if (stick_trigger)
    if (!destination_valid())
      destination_state=DESTINATION_INVALID_MOVE;
    else
      destination_state=DESTINATION_DONE;
}

/**
 * @brief show invalid destination move, delay, then return to show msg
 */
void destination_invalid_move(void)
{
    if (!frame_delay)
    msg(msg_cant_do_that);

  frame_delay--;
  frame_delay&=0x3f; // 64 frames

  if (!frame_delay)
    destination_state=DESTINATION_SHOW_MSG;
}

/**
 * @brief Done with destination, all valid, exit so move can be called.
 */
void destination_done(void)
{
  destination_x = cursor_x;
  destination_y = cursor_y;
  destination_piece = board_get(cursor_x,cursor_y);
  state=DESTINATION;
  destination_state=DESTINATION_SHOW_MSG;

}

/**
 * @brief select destination square
 */
void destination(void)
{
  switch(destination_state)
    {
    case DESTINATION_SHOW_MSG:
      destination_show_msg();
      break;
    case DESTINATION_CURSOR:
      destination_cursor();
      break;
    case DESTINATION_INVALID_MOVE:
      destination_invalid_move();
      break;
    case DESTINATION_DONE:
      destination_done();
      break;
    }
}
