/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Main program
 */

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

/**
 * @brief state machine variables
 */
State state=BOARD;

/**
 * @brief global frame variables
 */
unsigned char frame_counter, frame_delay;

void main(void)
{
  while(true)
    {
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
