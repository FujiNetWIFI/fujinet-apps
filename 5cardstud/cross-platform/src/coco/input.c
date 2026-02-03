#ifdef _CMOC_VERSION_
#include <coco.h>
#include "coco_bool.h"
#include "joystick.h"


uint8_t right_joy_state = JOY_NOT_USED;
uint8_t left_joy_state = JOY_NOT_USED;

unsigned char readJoystick() {
    byte value = 0;
    bool lbtn1, lbtn2, rbtn1, rbtn2;
    byte h, v;

    byte buttons = readJoystickButtons();   /* active-high */

    /* NOTE: As of right now, the enum in the coco.h */
    /* header file is incorrect for the button values. */
    lbtn1  = (buttons & 0x02 ) == 0;
    lbtn2  = (buttons & 0x08 ) == 0;
    rbtn1  = (buttons & 0x01 ) == 0;
    rbtn2  = (buttons & 0x04 ) == 0;

    // The first time a button is pressed,
    // ONLY register that the joystick is active
    // and return.
    if (left_joy_state == JOY_NOT_USED && (lbtn1 || lbtn2))
    {
        left_joy_state = JOY_SELECTING;
        right_joy_state = JOY_NOT_USED;
    }
    else if (left_joy_state == JOY_SELECTING && !(lbtn1 || lbtn2))
    {
        left_joy_state = JOY_USING;
        right_joy_state = JOY_NOT_USED;
    }

    if (right_joy_state == JOY_NOT_USED && (rbtn1 || rbtn2))
    {
        right_joy_state = JOY_SELECTING;
        left_joy_state = JOY_NOT_USED;
    }
    else if (right_joy_state == JOY_SELECTING && !(rbtn1 || rbtn2))
    {
        right_joy_state = JOY_USING;
        left_joy_state = JOY_NOT_USED;
    }

    // Don't read joystick positions until one of the buttons is pressed.
    if (left_joy_state == JOY_USING || right_joy_state == JOY_USING)
    {
        const byte *joy = readJoystickPositions();

       // Toggle back and forth between left and right joystick
       // depending on which one's buttons were last pressed.
        if (left_joy_state == JOY_USING) // Using left joystick
        {
            h = joy[JOYSTK_LEFT_HORIZ];
            v = joy[JOYSTK_LEFT_VERT];
            if (lbtn1)
                value |= 16; /* bit 4 = button 1 */
            if (lbtn2)
                value |= 32; /* bit 5 = button 2 */
        }
        else // Using right joystick
        {
            h = joy[JOYSTK_RIGHT_HORIZ];
            v = joy[JOYSTK_RIGHT_VERT];

            if (rbtn1)
                value |= 16; /* bit 4 = button 1 */
            if (rbtn2)
                value |= 32; /* bit 5 = button 2 */
        }

        /* Direction bits: UP, DOWN, LEFT, RIGHT
           Vertical: 0 = UP, 63 = DOWN */
        if (v <= JOY_LOW_TH)
            value |= 1; /* up */
        if (v >= JOY_HIGH_TH)
            value |= 2; /* down */
        if (h <= JOY_LOW_TH)
            value |= 4; /* left */
        if (h >= JOY_HIGH_TH)
            value |= 8; /* right */
    }
    else
    {
        value = 0;
    } 

    return value;
}

#endif
