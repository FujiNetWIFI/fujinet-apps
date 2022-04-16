/**
 * PLATOTERM for Coleco Adam
 *
 * @author Thomas Cherryhomes
 * @author Steve Peltz (protocol.c)
 * @license gpl v. 3, see COPYING for Details
 */

#include <stdbool.h>
#include "screen.h"
#include "terminal.h"
#include "keyboard.h"
#include "io.h"
#include "reboot.h"

bool running=false;

void main(void)
{
  screen_init();
  terminal_init();
  keyboard_init();
  io_init();

  running=true;
  
  while(running==true)
    {
      keyboard_main();
      running=io_main();
    }

  io_done();
  keyboard_done();
  terminal_done();
  screen_done();
  reboot();
}
