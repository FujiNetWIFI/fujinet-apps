/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Main program
 */

#include <stdbool.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "source.h"
#include "destination.h"
#include "check.h"
#include "move.h"

State state=BOARD;

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
    }
}
