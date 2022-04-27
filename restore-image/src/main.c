/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "select_host.h"
#include "select_file.h"
#include "select_tape.h"
#include "perform.h"
#include "done.h"

#include "typedefs.h"
#include "system.h"

State state=SELECT_HOST;

void main(void)
{
  screen_init();
  input_init();
  io_init();
  
  while (1)
    {
      switch(state)
	{
	case SELECT_HOST:
	  state=select_host();
	  break;
	case SELECT_FILE:
	  state=select_file();
	  break;
	case SELECT_TAPE:
	  state=select_tape();
	  break;
	case PERFORM:
	  state=perform();
	  break;
	case DONE:
	  state=done();
	  break;
	}
    }
}
