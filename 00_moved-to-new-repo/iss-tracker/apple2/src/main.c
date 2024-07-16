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
  clrscr();
  sp_init();
  net = sp_find_network();
  map();
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_apple2_mix(true);

  while (1)
    {
      timer=524088;
      clrscr();
      satellite_fetch(&lon,&lat,lon_s,lat_s,&ts);
      osd(lon_s,lat_s,ts);
      satellite_draw(lon,lat);

      while (timer>0)
        {
          if (kbhit())
            switch(cgetc())
              {
              case CH_ESC:
                return;
              case CH_ENTER:
                timer=0;
                break;
              default:
                break;
              }

          timer--;
        }

      satellite_erase();
    }
}
