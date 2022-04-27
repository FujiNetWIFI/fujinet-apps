/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "typedefs.h"

#ifdef BUILD_ATARI
#include "atari/screen.h"
#include "atari/input.h"
#include "atari/io.h"
#endif

#ifdef BUILD_ATARI
#include "screen.h"
#include "input.h"
#include "io.h"
#endif

State state=SELECT_HOST_SLOT;

void main(void)
{
  while (1)
    {
      switch(state)
	{
	case SELECT_HOST_SLOT:
	  state=select_host_slot();
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
