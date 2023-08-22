/** 
 * Input routines
 */

#include <msx.h>
#include <eos.h>
#include <smartkeys.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"
#include "cursor.h"

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
 * ADAM keyboard mapping
 */
#define KEY_BACKSPACE    0x08
#define KEY_TAB          0x09
#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x1B
#define KEY_SPACE        0x20
#define KEY_1            0x31
#define KEY_2            0x32
#define KEY_3            0x33
#define KEY_4            0x34
#define KEY_5            0x35
#define KEY_6            0x36
#define KEY_7            0x37
#define KEY_8            0x38
#define KEY_HOME         0x80
#define KEY_SMART_I      0x81
#define KEY_SMART_II     0x82
#define KEY_SMART_III    0x83
#define KEY_SMART_IV     0x84
#define KEY_SMART_V      0x85
#define KEY_SMART_VI     0x86
#define KEY_WILD_CARD    0x90
#define KEY_UNDO         0x91
#define KEY_MOVE         0x9A
#define KEY_GET          0x93
#define KEY_INSERT       0x94
#define KEY_PRINT        0x95
#define KEY_CLEAR        0x96
#define KEY_DELETE       0x97
#define KEY_COPY         0x92
#define KEY_STORE        0x9B
#define KEY_S_INSERT     0x9C
#define KEY_S_PRINT      0x9D
#define KEY_S_CLEAR      0x9E
#define KEY_S_DELETE     0x9F
#define KEY_UP_ARROW     0xA0
#define KEY_DOWN_ARROW   0xA2
#define KEY_C_UP_ARROW   0xA4
#define KEY_C_DOWN_ARROW 0xA6

static void input_clear_bottom(void)
{
  msx_vfill(0x1200,0x00,768);
}

void input_line(unsigned char x, unsigned char y, unsigned char o, char *c, unsigned char len, bool *keyVI)
{
  unsigned char pos=o;

  c += o;
  x += o;

  if (keyVI != NULL)
    *keyVI = false;
  
  cursor(true);
  input_clear_bottom();

  gotoxy(x,y);
  cursor_pos(x,y);

  while (key = eos_read_keyboard())
    {
      smartkeys_sound_play(SOUND_KEY_PRESS);
      if (key == KEY_RETURN)
	{
	  cursor(false);
	  break;
	}
      else if (key == KEY_BACKSPACE)
	{
	  if (pos > 0)
	    {
	      pos--;
	      x--;
	      c--;
	      *c=0x00;
	      putchar(KEY_BACKSPACE);
	      putchar(KEY_SPACE);
	      putchar(KEY_BACKSPACE);
	      cursor_pos(x,y);
	    }
	}
      else if ((keyVI != NULL) && (key == KEY_SMART_VI))
	{
	  *keyVI=true;
	  return;
	}
      else if (key > 0x1F && key < 0x7F) // Printable characters 
	{
	  if (pos < len)
	    {
	      pos++;
	      x++;
	      *c=key;
	      c++;
	      putchar(key);
	      cursor_pos(x,y);
	    }
	}
    }
  eos_end_read_keyboard();
  eos_start_read_keyboard();
}
