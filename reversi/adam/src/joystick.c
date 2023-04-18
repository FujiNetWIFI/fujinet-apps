#include <stdio.h>
#include <eos.h>
#include "joystick.h"

static GameControllerData cont;
static unsigned char joystick = 0;
static unsigned char button = 0;
static unsigned char keypad = 0;



int read_joystick(int *fire)
{

    int fire_button;
    
    fire_button = 1;

    eos_read_game_controller(READ_CONTROLLER_1 | READ_CONTROLLER_2, &cont);
    joystick = cont.joystick1 | cont.joystick2; // use value from either joystick
    button = cont.joystick1_button_left | cont.joystick1_button_right | cont.joystick2_button_left | cont.joystick2_button_right;
    keypad = cont.joystick1_keypad;

    if (button)
        fire_button = 0;

    *fire = fire_button;

    return joystick;
}
