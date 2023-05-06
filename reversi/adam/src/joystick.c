#include <stdio.h>
#include <msx.h>
#include <games.h>
#include "joystick.h"




int read_joystick(int *fire)
{
    int r, button,fire_button, pos;

    r = joystick(1) & 0xFF;

    button = r >= 16;
    r = r & 0x0F;
    
    pos = 0;
    if (r & 8)
        pos |= UP;

    if (r & 4)
        pos |= DOWN;

    if (r & 2)
        pos |= LEFT;

    if (r & 1)
        pos |= RIGHT;

    fire_button = ! button;

    *fire = fire_button;

    return pos;
}
