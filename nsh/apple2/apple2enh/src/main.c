/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Main entrypoint
 */

#include <apple2enh.h>
#include <stdbool.h>
#include <conio.h>
#include "page.h"
#include "state.h"
#include "splash.h"
#include "main_menu.h"
#include "directory.h"
#include "sp.h"

MainState mainState=SPLASH;
bool running=true;
char line[80]; /* temporary line storage for display formatting */

void done(void)
{
  clrscr();
}

void main(void)
{
  videomode(VIDEOMODE_80x24);

  sp_init();
  net[0] = sp_find_network(); // Temporary!
  
  while(running)
    {
      switch (mainState)
	{
	case SPLASH:
	  splash();
	  break;
	case MAIN_MENU:
	  main_menu();
	  break;
	case DIRECTORY:
	  directory();
	  break;
	case DIRECTORY_MAKE:
	  break;
	case DIRECTORY_DEL:
	  break;
	case DIRECTORY_GET:
	  break;
	case DIRECTORY_PUT:
	  break;
	case GET_URL:
	  break;
	case PUT_URL:
	  break;
	case NC:
	  break;
	case DONE:
	  done();
	  running=false;
	  break;
	}
    }
}
