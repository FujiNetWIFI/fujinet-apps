/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Cursor routines
 */

#include <stdlib.h>
#include <atari.h>
#include "typedefs.h"
#include "board.h"
#include "delay.h"

#define UP    14
#define DOWN  13
#define LEFT  11
#define RIGHT 7

/**
 * @brief delay value in VBLANKs
 */
#define CURSOR_DELAY 6

/**
 * @brief initial cursor position
 */
unsigned char cursor_y = 7;
unsigned char cursor_x = 3;

/**
 * @brief cursor delay
 */
static void cursor_delay(void)
{
  Piece p = board_get(cursor_x,cursor_y);

  board_set(cursor_x,cursor_y,CURSOR);
  
  delay(CURSOR_DELAY);
  
  board_set(cursor_x,cursor_y,p);

  delay(CURSOR_DELAY);
}

/**
 * @brief set cursor position
 */
void cursor_pos(char dx, char dy)
{
  signed char nx = (signed char)cursor_x+dx;
  signed char ny = (signed char)cursor_y+dy;
  
  if (nx<0)
    nx=0;
  else if (nx>3)
    nx=3;

  if (ny<0)
    ny=0;
  else if (ny>7)
    ny=7;
  
  cursor_x = nx;
  cursor_y = ny;
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
 * @brief Move and position cursor, exit on trigger
 */
void cursor(void)
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
  
  cursor_delay();
}
