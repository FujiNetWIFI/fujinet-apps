/**
 * @brief   Homesoft Launcher
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <stdbool.h>
#include "state.h"
#include "init.h"
#include "select.h"
#include "search.h"
#include "mount.h"
#include "boot.h"

State state;

void main(void)
{
  while (true)
    {
    switch (state)
      {
      case INIT:
	init();
	break;
      case SELECT:
	select();
	break;
      case SEARCH:
	search();
	break;
      case MOUNT:
	mount();
	break;
      case BOOT:
	boot();
	break;
	}
    }
}
