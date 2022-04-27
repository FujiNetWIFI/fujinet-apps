/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include "io.h"

HostSlots hs;

void io_init(void)
{
}

HostSlots *io_select_host(void)
{
  return &hs;
}
