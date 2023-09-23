/**
 * @brief   High Level input functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#ifndef INPUT_H
#define INPUT_H

#include <conio.h>
#include <joystick.h>

/* ********************************************************************
 * Functions that must be implemented by each platform
 * ********************************************************************/

unsigned char readJoystick();

/* ******************************************************************** 
 * Cross platform input functions
 * ********************************************************************/

void clearInputQueue() {
  while (kbhit()) 
    cgetc();
}

/// @brief Returns either a valid joystick mask (using the platform specific keymap.h), failing that the raw key code.
/// Works on assumption that there is no overlap between joystick and other keys. Can modify if needed.
/// @return value to be checked against JOY_X_MASK, failing that, a raw key code, failing that, 0 if no input detected.
unsigned char readCommonInput() {
  static unsigned char lastJoy, val;
  
  val = readJoystick();

  if (val != lastJoy) {
    return lastJoy = val;
  }

  switch (val = kbhit() ? cgetc() : 0) {
    case KEY_LEFT_ARROW: case KEY_LEFT_ARROW_2: case KEY_LEFT_ARROW_3:
      return JOY_LEFT_MASK;

    case KEY_RIGHT_ARROW: case KEY_RIGHT_ARROW_2: case KEY_RIGHT_ARROW_3:
      return JOY_RIGHT_MASK;

    case KEY_UP_ARROW: case KEY_UP_ARROW_2: case KEY_UP_ARROW_3:
      return JOY_UP_MASK;

    case KEY_DOWN_ARROW: case KEY_DOWN_ARROW_2: case KEY_DOWN_ARROW_3:
      return JOY_DOWN_MASK;

    case KEY_FIRE_RETURN: case KEY_FIRE_SPACE:
      return JOY_BTN_1_MASK;
  }

  return val;
}

#endif /* INPUT_H */
