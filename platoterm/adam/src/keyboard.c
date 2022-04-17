/**
 * PLATOTERM for Coleco Adam
 *
 * @author Thomas Cherryhomes
 * @author Steve Peltz (protocol.c)
 * @license gpl v. 3, see COPYING for Details
 */

#include <eos.h>
#include <smartkeys.h>
#include "protocol.h"
#include "plato_key.h"
#include "io.h"
#include "key.h"

static unsigned char ch;

/**
 * Initialize Keyboard
 */
void keyboard_init(void)
{
  eos_start_read_keyboard();
}

/**
 * A simple key press feedback.
 */
void keyboard_click(void)
{
  smartkeys_sound_play(SOUND_TYPEWRITER_CLACK);
}

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(unsigned char platoKey)
{
  if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;

}

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  ch=eos_end_read_keyboard();

  if (ch>1)
    {
      keyboard_click();
      keyboard_out(key_to_pkey[ch]);
      eos_start_read_keyboard();
    }
}

/**
 * keyboard_out_tty - keyboard output to serial I/O in TTY mode
 */
void keyboard_out_tty(char ch)
{
  io_send_byte(ch);
}

void keyboard_done(void)
{
  eos_end_read_keyboard();
}
