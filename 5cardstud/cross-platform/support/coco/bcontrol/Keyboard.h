/*  Keyboard.h - Autonomous keyboard functions for the CoCo.
    By Pierre Sarrazin <http://sarrazip.com/>
    This file is in the public domain.
*/

#ifndef _H_Keyboard
#define _H_Keyboard

#include <coco.h>

#ifdef OS9
#include "OS9RawTerminal.h"
#endif


typedef struct Keyboard
{
    #ifdef OS9
    OS9RawTerminal os9RawTerminal;
    #else
    byte keyBuffer[8];
    byte caseFlag;
    #endif
} Keyboard;


// Only a single instance of Keyboard must be initialized.
// Call Keyboard_shutdown() before the program ends.
//
// Under OS-9, this puts the terminal in "no echo" mode and disables
// Ctrl-C (Break) and Ctrl-E so they do not interrupt or abort the program.
// Make sure to call Keyboard_shutdown() before the program ends so that
// the normal terminal mode is restored.
//
void Keyboard_init(Keyboard *keyboard);


// Returns 0 if no key is pressed.
// Using the CoCo 3 keys (Ctrl, Alt, F1, F2), all codes from 1 to 127
// can be produced:
//    - The Ctrl key resets bits 5 and 6.
//      Pressing it with A to Z gives ASCII codes 1 to 26.
//      Code 27: Ctrl-Shift-Down.
//      Code 28: Ctrl-Shift-Clear.
//      Code 29: Ctrl-Shift-Right.
//      Code 30: Ctrl-Up.
//      Code 31: Ctrl-Shift-Up.
//    - The F1 and F2 keys return ASCII codes 123 ({) and 125 (}).
//      When shifted, they return 124 (|) and 126 (~).
//    - Shift-Break produces ASCII code 127 (DEL).
//    - Shift-@ produces ASCII code 96 (`).
//    - Shift-Left produces 21 (NAK, i.e., kill line).
//    - Code 21 can also be produced with Ctrl-U.
//    - Code 19 can be produced with Ctrl-S.
//    - The Alt key sets bit 7.
//    - Code 128 can be produced with Ctrl-Alt-Shift-@.
//    - Shift-0 is the caps lock key.
//
// N.B.: To determine if a key is pressed or not, call isKeyPressed(),
//       which comes with CMOC and is declared in <coco.h>.
//       It does not rely on Color Basic.
//
byte Keyboard_poll(Keyboard *keyboard);


// To be called when usage of the Keyboard object is finished.
//
void Keyboard_shutdown(Keyboard *keyboard);


#endif  /* _H_Keyboard */
