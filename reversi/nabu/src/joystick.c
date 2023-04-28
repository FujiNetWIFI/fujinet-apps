#include <stdio.h>
#include <msx.h>
#include <games.h>
#include "joystick.h"




int read_joystick(int *fire)
{
    int r1, r2, button1=0, button2=0,fire_button=1, pos;

    r1 = joystick(1) & 0xFF;
    r2 = joystick(1) & 0xFF;

    button1 = r1 >= 16;
    button2 = r2 >= 16;
    r1 = r1 & 0x0F;
    
    pos = 0;
    if (r1 & 8)
        pos |= UP;

    if (r1 & 4)
        pos |= DOWN;

    if (r1 & 2)
        pos |= LEFT;

    if (r1 & 1)
        pos |= RIGHT;



    if (pos == 0)
    {
        if (r2 & 8)
            pos |= UP;

        if (r2 & 4)
            pos |= DOWN;

        if (r2 & 2)
            pos |= LEFT;

        if (r2 & 1)
            pos |= RIGHT;
        
    }

    if (button1)
        fire_button = !button1;
    else
        if (button2)
            fire_button = !button2;

    *fire = fire_button;

    return pos;
}
