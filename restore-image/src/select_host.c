/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "system.h"
#include "select_host.h"

unsigned char selected_host_slot;

State select_host(void)
{
  screen_select_host(io_select_host());

  while(1)
    {
      if (input_select_host())
	break;
    }

  return SELECT_FILE;
}
