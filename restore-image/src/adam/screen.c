/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <smartkeys.h>
#include <eos.h>
#include <msx.h>
#include <conio.h>

#include "screen.h"

void screen_init(void)
{
  smartkeys_set_mode();
}

void screen_select_host(HostSlots *hs)
{
}
