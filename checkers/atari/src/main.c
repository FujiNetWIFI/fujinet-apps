/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Main program
 */

#include <stdlib.h>
#include <atari.h>
#include <stdbool.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "source.h"
#include "destination.h"
#include "check.h"
#include "move.h"
#include "msg.h"

State state=BOARD;

unsigned char frame_counter;

/**
 * @brief Show current frame # in msg display
 */
void frame_debug(void)
{
  char tmp[4];

  itoa(frame_counter,tmp,10);
  
  msg(tmp);
}

void main(void)
{
  while(true)
    {

      frame_debug();

      switch(state)
	{
	case BOARD:
	  board();
	  break;
	case SOURCE:
	  source();
	  break;
	case DESTINATION:
	  destination();
	  break;
	case CHECK:
	  check();
	  break;
	case MOVE:
	  move();
	  break;
	}

      waitvsync();
      frame_counter++;
    }
}
