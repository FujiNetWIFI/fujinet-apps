/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Stick/Trigger routines
 */

#include <atari.h>
#include "typedefs.h"

/**
 * @brief stick direction
 */
Direction stick_direction;

/**
 * @brief stick trigger
 */
Trigger stick_trigger;

/**
 * @brief update stick state
 */
void stick(void)
{
  stick_direction = (Direction)OS.stick0;
  stick_trigger = (Trigger)!OS.strig0;    // Invert this.
}
