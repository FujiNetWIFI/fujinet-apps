/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Cursor routines
 */

#include <atari.h>
#include <stdbool.h>
#include "typedefs.h"
#include "board.h"

#define UP    14
#define DOWN  13
#define LEFT  11
#define RIGHT 7

extern unsigned char frame_counter;

/**
 * @brief initial cursor position
 */
unsigned char cursor_y;
unsigned char cursor_x;

/**
 * @brief piece under cursor
 */
Piece cursor_piece;

/**
 * @brief display cursor this frame
 */
bool cursor_visible=false;

/**
 * @brief set cursor position
 */
void cursor_pos(char dx, char dy)
{
  signed char nx = (signed char)cursor_x+dx;
  signed char ny = (signed char)cursor_y+dy;

  // Keep within the board bounds
  
  if (nx<0)
    nx=0;
  else if (nx>3)
    nx=3;

  if (ny<0)
    ny=0;
  else if (ny>7)
    ny=7;

  // We're within the board bounds, first ensure we've painted the piece under cursor

  board_set(cursor_x,cursor_y,cursor_piece);

  // Then update to new cursor position
  
  cursor_x = nx;
  cursor_y = ny;
  cursor_piece = board_get(cursor_x,cursor_y);
}

/**
 * @brief get piece at cursor
 * @return piece at cursor (see typedefs.h)
 */
Piece cursor_get(void)
{
  return board_get(cursor_x,cursor_y);
}

/**
 * @brief update cursor graphic
 */
void cursor_update(void)
{
  if (!(frame_counter & 7)) // Every 8 frames
    cursor_visible = !cursor_visible; // flip whether we are displaying cursor or piece

  if (cursor_visible)
    board_set(cursor_x,cursor_y,CURSOR);
  else
    board_set(cursor_x,cursor_y,cursor_piece);
}

/**
 * @brief Move and position cursor, exit on trigger
 */
void cursor(void)
{
  cursor_update();

  if (!(frame_counter & 15)) // run every 16 frames.
    {
      switch(OS.stick0)
	{
	case UP:
	  cursor_pos(0,-1);
	  break;
	case DOWN:
	  cursor_pos(0,1);
	  break;
	case LEFT:
	  cursor_pos(-1,0);
	  break;
	case RIGHT:
	  cursor_pos(1,0);
	  break;
	}
    }
}
