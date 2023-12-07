/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Stick/Trigger routines
 */

#ifndef STICK_H
#define STICK_H

#include "typedefs.h"

/**
 * @brief stick direction
 */
extern Direction stick_direction;

/**
 * @brief stick trigger
 */
extern Trigger stick_trigger;

/**
 * @brief update stick state
 */
void stick(void);

#endif /* STICK_H */
