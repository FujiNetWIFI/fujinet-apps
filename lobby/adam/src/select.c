/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <eos.h>
#include <video/tms99x8.h>
#include <smartkeys.h>
#include "select.h"
#include "state.h"
#include "input.h"
#include "bar.h"

extern State state;
extern int numEntries;

void select(void)
{
  bar_set(0,1,numEntries,0);

  smartkeys_display(NULL,NULL," CHANGE\n  NAME"," SHOUT","  CHAT"," REFRESH");
  smartkeys_status("\n  SELECT A ROOM\n");
  smartkeys_sound_play(SOUND_MODE_CHANGE);

  eos_start_read_keyboard();
  
  while (state==SELECT)
  {
      switch(input())
      {
        case KEY_UP_ARROW:
          bar_up();
        break;
        case KEY_DOWN_ARROW:
          bar_down();
        break;
        case KEY_SMART_V:
          state=CHAT;
        break;
        case KEY_SMART_VI:
          state=FETCH;
        break;
      }
  }

  eos_end_read_keyboard();
  
}
