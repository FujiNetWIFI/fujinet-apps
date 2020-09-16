/**
 * main for cc65 boot disk template
 *
 * Author:
 *  Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL 3.0, read LICENSE in top
 * level folder for details.
 */

#include <atari.h>
#include <stdlib.h>
#include "font.h"
#include "dlist.h"
#include "screen.h"
#include "menu.h"

void main(void)
{
  font_setup();
  dlist_setup();
  screen_setup();
  menu_main(0);
  
  for (;;) { }
}
