#ifndef _JOYSTICK_H
#define _JOYSTICK_H


#ifdef _CMOC_VERSION_
#include <coco.h>

// NOT FUNCTIONAL, YET

 /* Argument for the joy_read function */
#define JOY_1                   0
#define JOY_2                   1

/* Macros that evaluate the return code of joy_read */
#define JOY_UP(v)               ((v) & 0)
#define JOY_DOWN(v)             ((v) & 0)
#define JOY_LEFT(v)             ((v) & 0)
#define JOY_RIGHT(v)            ((v) & 0)
#define JOY_BTN_1(v)            ((v) & 0)      /* Universally available */
#define JOY_BTN_2(v)            ((v) & 0)      /* Second button if available */

#endif

#endif