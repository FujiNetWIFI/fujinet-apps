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

#define UP_LEFT    10
#define DOWN_LEFT  9
#define UP_RIGHT   6
#define DOWN_RIGHT 5

/**
 * @brief delay value in VBLANKs
 */
#define CURSOR_DELAY 8

/**
 * @brief initial cursor position
 */
Position _pos = 31;

/**
 * @brief cursor delay
 */
static void cursor_delay(void)
{
  unsigned char i;
  Piece p = board_get(_pos);

  board_set(_pos,CURSOR);
  
  for (i=0;i<CURSOR_DELAY;i++)
    waitvsync();

  board_set(_pos,p);

  for (i=0;i<CURSOR_DELAY;i++)
    waitvsync();
}

/**
 * @brief set cursor position
 */
void cursor_pos(Position o)
{
  Position p;
  char tmp[4];

  p = _pos + o;
  
  if (p<0)
    return;
  else if (p>31)
    return;

  if ((p >> 2) % 1)
    p++;
  _pos = p;

  itoa(p,tmp,10);
  msg(tmp);
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
	case UP_LEFT:
	  cursor_pos(-5);
	  break;
	case UP_RIGHT:
	  cursor_pos(-4);
	  break;
	case DOWN_LEFT:
	  cursor_pos(5);
	  break;
	case DOWN_RIGHT:
	  cursor_pos(4);
	  break;
	}
      
      cursor_delay();
    }
}
