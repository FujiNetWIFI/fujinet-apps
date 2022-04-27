/**
 * restore - copy an image to a target disk
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifdef BUILD_ADAM

#include <conio.h>
#include "die.h"

void die(char *c)
{
  gotoxy(0,23); cprintf(c);
  while(1) {}
}

#endif /* BUILD_ADAM */
