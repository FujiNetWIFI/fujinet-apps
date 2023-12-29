/** 
 * Input routines
 */

#include <video/tms99x8.h>
#include <eos.h>
#include <smartkeys.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"

static GameControllerData cont;
static unsigned char key=0;
static unsigned char joystick=0;
static unsigned char joystick_copy=0;
static unsigned char button=0;
static unsigned char button_copy=0;
static unsigned char keypad=0;
static unsigned char keypad_copy=0;
static unsigned char repeat=0;
static int entry_timer=0;

/**
 *  input init 
 */
void input_init(void)
{
  eos_start_read_keyboard();
}  

/**
 * Get input from keyboard/joystick
 * @return keycode (or synthesized keycode if joystick)
 */
unsigned char input()
{
  key = eos_end_read_keyboard();
  if (entry_timer>0)
    entry_timer--;

  if (key > 1)
    {
      eos_start_read_keyboard();
      if (key != 0x0D && key != 0x1B)
	smartkeys_sound_play(SOUND_KEY_PRESS);
      return key;
    }
  else
    {
      eos_read_game_controller(0x03,&cont);
      joystick = cont.joystick1 | cont.joystick2;
      button = cont.joystick1_button_left | cont.joystick1_button_right | cont.joystick2_button_left | cont.joystick2_button_right;
      keypad = cont.joystick1_keypad;

      if ((button > 0) && (button != button_copy))
	{
	  key=0x0d; // same as RETURN
	}
      else if ((keypad != 0x0F) && (keypad != keypad_copy))
	{
	  switch (keypad)
	    {
	    case 1: // Slot 1
	      key=KEY_1;
	      break;
	    case 2: // Slot 2
	      key=KEY_2;
	      break;
	    case 3: // Slot 3
	      key=KEY_3;
	      break;
	    case 4: // Slot 4
	      key=KEY_4;
	      break;
	    case 5: // Slot 5
	      key=KEY_5;
	      break;
	    case 6: // Slot 6
	      key=KEY_6;
	      break;
	    case 7: // Slot 7
	      key=KEY_7;
	      break;
	    case 8: // Slot 8
	      key=KEY_8;
	      break;
	    case 0x0a: // *
	      key=KEY_SMART_VI;
	      break;
	    }
	}
      else if ((joystick > 0) && (joystick == joystick_copy))
	repeat++;
      else
	repeat=0;

      if (repeat > 16)
	{
	  repeat=0;
	  switch(joystick)
	    {
	    case 1: // UP
	      key=KEY_UP_ARROW;
	      break;
	    case 4: // DOWN
	      key=KEY_DOWN_ARROW;
	      break;
	    }
	}
      
      joystick_copy = joystick;
      button_copy = button;
      keypad_copy = keypad;
    }
  
  return key;
}
