/**
 * ISS Tracker
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <conio.h>
#include <tgi.h>
#include <apple2.h>
#include <stdbool.h>
#include "map.h"
#include "satellite.h"
#include "osd.h"
#include "sp.h"

unsigned char net;
char lon_s[16], lat_s[16];
int lat, lon;
long timer;
unsigned long ts;

void main(void)
{
  sp_init();
  net = sp_find_network();
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_apple2_mix(true);
  tgi_clear();

  while (1)
    {
      timer=524088;
      clrscr();
      satellite_fetch(&lon,&lat,lon_s,lat_s,&ts);
      map();
      osd(lon_s,lat_s,ts);
      satellite(lon,lat);

      while (timer>0)
	{
	  if (kbhit())
	    switch(cgetc())
	      {
	      case 0x1b:
		return;
	      case 0x0D:
	      case 0x0A:
		timer=0;
		break;
	      default:
		break;
	      }

	  timer--;
	}
    }
}
