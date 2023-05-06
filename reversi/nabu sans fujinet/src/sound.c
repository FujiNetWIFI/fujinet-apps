
#define PROTOTYPES_ONLY
#include "sound.h"
#include <NABU-LIB.h>

void sound_init(void)
{
    initNABULIBAudio();
}

void sound_negative_beep(void)
{
    playNoteDelay(0, 74, 74);
}

void sound_chime(void)
{
    playNoteDelay(0, 50, 50);
}

void sound_mode_change(void)
{
    playNoteDelay(0, 50, 50);
}

void sound_confirm(void)
{
    playNoteDelay(0, 50, 50);
}

