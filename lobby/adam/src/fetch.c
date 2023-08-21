/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <stdlib.h>
#include <smartkeys.h>
#include "fetch.h"
#include "state.h"

extern State state;

void fetch(void)
{
  smartkeys_clear();
  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("\n  FETCHING ROOM LIST...");
  smartkeys_sound_play(SOUND_CONFIRM);

  
  
  state=DISPLAY;
}
