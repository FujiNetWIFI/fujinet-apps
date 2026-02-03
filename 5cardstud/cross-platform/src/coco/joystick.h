#ifdef _CMOC_VERSION_

/**
 * @brief Coco input routines
 * @author Thomas Cherryhomes
 * @license gpl v.3
 */

#ifndef _JOYSTICK_H
#define _JOYSTICK_H

/*****************************************************************************/
/*                              Joystick Definitions                         */
/*****************************************************************************/

#define JOY_CENTER   31
#define JOY_HALF     16

#define JOY_LOW_TH   (JOY_CENTER - JOY_HALF)   /* 15 */
#define JOY_HIGH_TH  (JOY_CENTER + JOY_HALF)   /* 47 */

#define JOY_NOT_USED 0
#define JOY_SELECTING 1
#define JOY_USING 2

/* Macros that evaluate the return code of joy_read */
#define JOY_UP(v) ((v) & 1)
#define JOY_DOWN(v) ((v) & 2)
#define JOY_LEFT(v) ((v) & 4)
#define JOY_RIGHT(v) ((v) & 8)
#define JOY_BTN_1(v) ((v) & 16) /* Universally available */
#define JOY_BTN_2(v) ((v) & 32) /* Second button if available */

/* End of joystick.h */
#endif

#endif
