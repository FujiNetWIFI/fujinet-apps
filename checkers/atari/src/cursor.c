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
#include "msg.h"

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
unsigned char _y = 7;
unsigned char _x = 3;

/**
 * @brief cursor delay
 */
static void cursor_delay(void)
{
  unsigned char i;
  Piece p = board_get(_x,_y);

  board_set(_x,_y,CURSOR);
  
  for (i=0;i<CURSOR_DELAY;i++)
    waitvsync();

  board_set(_x,_y,p);

  for (i=0;i<(CURSOR_DELAY<<1);i++)
    waitvsync();
}

/**
 * @brief set cursor position
 */
void cursor_pos(char dx, char dy)
{
  signed char nx = (signed char)_x+dx;
  signed char ny = (signed char)_y+dy;
  
  if (nx<0)
    nx=0;
  else if (nx>3)
    nx=3;

  if (ny<0)
    ny=0;
  else if (ny>7)
    ny=7;
  
  _x = nx;
  _y = ny;
}

/**
 * @brief Move and position cursor, exit on trigger
 */
void cursor(void)
{
  while (OS.strig0)
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
}
