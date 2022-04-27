/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include "../typedefs.h"

void screen_init(void);
void screen_select_host(HostSlots *hs);
void screen_bar(char y, char c, char m, char i, bool onOff);

#endif /* SCREEN_H */
