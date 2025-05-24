/**
 * @brief FujiLink, a terminal emulator for FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose main()
 */

#include <smartkeys.h>
#include <stdbool.h>
#include "globals.h"
#include "splash.h"
#include "directory.h"
#include "term.h"

void main(void)
{
    smartkeys_sound_init();
    
  state=SPLASH;
  while (true)
    {
      switch(state)
	{
	case SPLASH:
	  splash();
	  break;
	case DIRECTORY:
	  directory();
	  break;
	case TERM:
	  term();
	  break;
	}
    }
}
