/**
 * @brief FujiNet Lobby for Adam
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <video/tms99x8.h>
#include <eos.h>
#include <smartkeys.h>
#include <conio.h>
#include "init.h"
#include "state.h"

extern State state;

void init(void)
{
  smartkeys_sound_init();
  smartkeys_set_mode();

  smartkeys_display(NULL,NULL,NULL,NULL,NULL,NULL);
  smartkeys_status("\n  WELCOME TO THE #FUJINET LOBBY.");

  smartkeys_sound_play(SOUND_POSITIVE_CHIME);
  sleep(1);

  state=USERNAME;
}
