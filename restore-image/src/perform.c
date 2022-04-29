/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <string.h>
#include "perform.h"
#include "typedefs.h"
#include "system.h"

extern char path[256];
unsigned char src[16384], dst[16384];
PFState pfState;

void perform_init(void)
{
  io_perform_open(path);
  while(1);
}

void perform_confirm(void)
{
  screen_perform_confirm();
  
  if (input_perform_confirm())
    pfState=PF_RESTORE;
  else
    pfState=PF_ABORT;
}

State perform(void)
{
  pfState = PF_INIT;
  
  while(1)
    {
      switch(pfState)
	{
	case PF_INIT:
	  perform_init();
	  break;
	case PF_CONFIRM:
	  break;
	case PF_RESTORE:
	  break;
	case PF_ABORT:
	  break;
	case PF_DONE:
	  break;
	}
    }
  return DONE;
}
