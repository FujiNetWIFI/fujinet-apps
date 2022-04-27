/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef IO_H
#define IO_H

#include "../typedefs.h"

void io_init(void);
HostSlots *io_select_host(void);

#endif /* IO_H */
