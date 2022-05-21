/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include "state.h"
#include "init.h"
#include "fetch.h"
#include "display.h"
#include "track.h"
#include "who.h"

State state = INIT;

void main(void)
{
  while (1)
    {
      switch(state)
	{
	case INIT:
	  state = init();
	  break;
	case FETCH:
	  state = fetch();
	  break;
	case DISPLAY:
	  state = display();
	  break;
	case TRACK:
	  state = track();
	  break;
	case WHO:
	  state = who();
	  break;
	}
    }
}
