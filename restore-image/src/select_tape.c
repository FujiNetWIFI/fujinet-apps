/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "select_tape.h"
#include "system.h"

unsigned char selected_tape=0;

State select_tape(void)
{
  while (selected_tape==0)
    {
      screen_select_tape();
      input_select_tape();
    }
  
  return PERFORM;
}
