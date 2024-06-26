/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose main
 */

#include <stdlib.h>
#include <stdbool.h>
#include "video.h"
#include "defines.h"
#include "location.h"
#include "weather.h"

State state=LOCATION;

void main(void)
{
  video_setup();

  while(true)
    {
      switch(state)
	{
	case LOCATION:
	  location();
	  break;
	case WEATHER:
	  weather();
	  break;
	case EXIT:
	  exit(0);
	}
    }
}
