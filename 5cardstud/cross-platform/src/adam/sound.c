#ifdef __ADAM__

#include <stdint.h>
#include "smartkeys.h"

/*
  Platform specific sound functions 
*/
// #define SOUND_TYPEWRITER_CLACK 1
// #define SOUND_CONFIRM 2
// #define SOUND_POSITIVE_CHIME 3
// #define SOUND_OOPS 4
// #define SOUND_DOUBLE_CHIME 5
// #define SOUND_LONG_BEEP 6
// #define SOUND_KEY_PRESS 7
// #define SOUND_NEGATIVE_BUZZ 8
// #define SOUND_MODE_CHANGE 9
void initSound()
{
  smartkeys_sound_init();
}

void soundJoinGame()
{
  smartkeys_sound_play(SOUND_CONFIRM);
}

void soundMyTurn()
{
  smartkeys_sound_play(SOUND_POSITIVE_CHIME);
}

void soundGameDone()
{
  smartkeys_sound_play(SOUND_LONG_BEEP);
}

void soundDealCard()
{
    smartkeys_sound_play(SOUND_TYPEWRITER_CLACK);

}

void soundTick()
{
  smartkeys_sound_play(SOUND_KEY_PRESS);
}

void soundPlayerJoin()
{
  smartkeys_sound_play(SOUND_CONFIRM);
}

void soundPlayerLeft()
{
  smartkeys_sound_play(SOUND_KEY_PRESS);
}

void soundSelectMove()
{
  smartkeys_sound_play(SOUND_KEY_PRESS);
}

void soundCursor()
{
  smartkeys_sound_play(SOUND_KEY_PRESS);
}

void soundCursorInvalid()
{
  smartkeys_sound_play(SOUND_NEGATIVE_BUZZ);
}

void soundTakeChip(uint16_t counter)
{
  smartkeys_sound_play(SOUND_KEY_PRESS);
}

#endif /* __ADAM__ */
