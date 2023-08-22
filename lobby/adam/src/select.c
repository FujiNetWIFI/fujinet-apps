/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <msx.h>
#include <smartkeys.h>
#include "select.h"
#include "state.h"
#include "input.h"

extern State state;

void select(void)
{
  smartkeys_display(NULL,NULL," CHANGE\n  NAME"," SHOUT","  CHAT"," REFRESH");
  smartkeys_status("\n  SELECT A ROOM\n");
  smartkeys_sound_play(SOUND_MODE_CHANGE);

  while (state==SELECT)
    {
      switch(input())
	{
	case KEY_UP_ARROW:
	  break;
	case KEY_DOWN_ARROW:
	  break;
	}
    }
  
  state=HALT;
}
