/*  Joystick.h - Autonomous joystick functions for the CoCo.
    By Pierre Sarrazin <http://sarrazip.com/>
    This file is in the public domain.
*/

#ifndef _H_Joystick
#define _H_Joystick

#include <coco.h>


#ifndef OS9  /* See Joystick_getAxisValuesAndButtonState() for a portable function. */


// Must be called while the analog multiplexer is disabled, otherwise
// a buzzing sound may be audible while polling the joystick.
// (That multiplexer is disabled when Color Basic's first OK prompt appears.)
// See Joystick_disableSound() and Joystick_enableSound().
//
// axisValues[]: The array elements that correspond to the axes selected
//               by axisSelectionMask receive values from 0 to 63.
// axisSelectionMask: Set bit 0..3 to read the position of axes 0..3 respectively.
//                    The horizontal and vertical axes of the right joystick
//                    correspond to bits 0 and 1 of axisSelectionMask and their
//                    positions will be stored in axisValues[0] and axisValues[1].
//
// Example:
//   byte axisValues[4];
//   Joystick_getAxisValues(axisValues, 0x03);  // read right joystick axes only
//
// The technique used by this function is liable to be slow.
// Joystick_selectAnalogMuxInputs() and Joystick_compareAxis() can be used
// to achieve a faster read if fewer than 6 bits of precision are required,
// e.g., just determining if the horizontal axis of a stick is below level 16 or not.
//
void Joystick_getAxisValues(byte axisValues[4], byte axisSelectionMask);


// Returns a 4 bit value that indicates if the four supported
// joystick buttons are up or down.
// A set bit means the button is up. A reset bit means the button is down.
// Bit 0 represents the first button of the right joystick.
// Bit 1 represents the second button of the right joystick.
// Bit 2 represents the first button of the left joystick.
// Bit 3 represents the second button of the left joystick.
//
// Must be called while the analog multiplexer is disabled, otherwise
// a buzzing sound may be audible while polling the joystick.
// See Joystick_disableSound() and Joystick_enableSound().
//
// This function may not work if called during an IRQ service routine.
//
byte Joystick_getButtonStates(void);


// Selects the joystick axis to be read by Joystick_compareAxis().
// axis: 0 = horizontal axis of the right joystick;
//       1 = vertical axis of the right joystick;
//       2 = horizontal axis of the left joystick;
//       3 = vertical axis of the left joystick.
//
void Joystick_selectAnalogMuxInputs(byte axis);


// Indicates if the joystick axis selected by the last call to 
// Joystick_selectAnalogMuxInputs() is higher-or-equal or below the
// given reference value.
// referenceValue: 0..63.
// Returns TRUE if higher-or-equal, FALSE otherwise.
// 
BOOL Joystick_compareAxis(byte referenceValue);


// Disables the analog multiplexer.
// 
void Joystick_disableSound(void);


// Enables the analog multiplexer.
// 
void Joystick_enableSound(void);


#endif


// Polls the selected joystick and reports its horizontal and vertical positions
// and the state of its button.
// joystickNo: 0 for right joystick, 1 for left joystick.
// axisValues[]: Element [0] receives 0..63 as the horizontal position
//               (0 means full left, 63 means full right).
//               Element [1] receives 0..63 as the vertical position
//               (0 means top, 63 means bottom).
// Returns 1 if the button is up, or 0 if it is down.
//
// N.B.: Under the Basic environment (i.e., accessing the hardware directly),
//       Joystick_disableSound() must be called before calling this function,
//       and Joystick_enableSound() must be called after.
//       This is not needed under OS-9.
//
byte Joystick_getAxisValuesAndButtonState(byte joystickNo, byte axisValues[2]);


#endif  /* _H_Joystick */
