/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.c - Keyboard functions (c64)
 */

#include <stdint.h>
#include <peekpoke.h>
#include <conio.h>
#include "../screen.h"
#include "../prefs.h"
#include "../keyboard.h"
#include "../protocol.h"
#include "key.h"

static uint8_t lastkey;
extern padBool TTY;

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  uint8_t modifier=PEEK(0x28D);
  uint8_t key=PEEK(0xCB);

  if (key<0x40)
    {
      // Handle Function keys
      if (key==0x04)
	{
	  if (lastkey!=key)
	    {
	      // Change colors
	      switch (modifier)
		{
		case MODIFIER_NONE:	// f1
		  screen_cycle_background();
		  break;
		case MODIFIER_SHIFT:	// f2
		  screen_cycle_foreground();
		  break;
		case MODIFIER_COMMO:	// logo + f1
		  screen_cycle_border();
		}
	      screen_update_colors();
	      keyboard_clear();
	    }
	}
      else if (key==0x05)		// f3
	{
	  if (lastkey!=key)
	    {
	      prefs_run();
	    }
	}
      // Handle other keys
      else if (TTY)
	{
	  if (kbhit())
	    {
	      keyboard_out_tty(cgetc());
	    }
	}
      else if (lastkey!=key)
	{
	  switch (modifier)
	    {
	    case MODIFIER_NONE:
	      keyboard_out(KEYBOARD_TO_PLATO[key]);
	      break;
	    case MODIFIER_SHIFT:
	      keyboard_out(KEYBOARD_TO_PLATO_SHIFT[key]);
	      break;
	    case MODIFIER_COMMO:
	      keyboard_out(KEYBOARD_TO_PLATO_COMMO[key]);
	      break;
	    case MODIFIER_COMMO_SHIFT:
	      keyboard_out(KEYBOARD_TO_PLATO_CS[key]);
	      break;
	    case MODIFIER_CTRL:
	      keyboard_out(KEYBOARD_TO_PLATO_CTRL[key]);
	      break;
	    case MODIFIER_CTRL_SHIFT:
	      keyboard_out(KEYBOARD_TO_PLATO_CTRL_SHIFT[key]);
	    }
	}
    }
  lastkey=key;
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{
  POKE(0xC6,0);
}
