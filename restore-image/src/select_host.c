/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "system.h"
#include "select_host.h"
#include "bar.h"

char selected_host_slot;
char hs[8][32];

State select_host(void)
{
  selected_host_slot=0;
  io_select_host();
  screen_select_host();

  bar_init(1,1,8,0);

  while(1)
    {
      
      if (input_select_host(&selected_host_slot))
	break;
      bar_set(selected_host_slot);
    }

  bar_done();
  
  return SELECT_FILE;
}
