/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief main()
 */

#include <vic20.h>
#include <tgi.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "map.h"
#include "fetch.h"
#include "osd.h"
#include "satellite.h"

char lon_s[16], lat_s[16];
int lon, lat;
time_t ts;

const unsigned char pal[2] = {6,5};

void main(void)
{
  unsigned short i;
  
  tgi_install(vic20_hi_tgi);
  tgi_init();
  tgi_clear();
  tgi_setpalette(pal);
  VIC.bg_border_color=0x66;
  
  while(true)
    {
      map();
      fetch(&lon,&lat,lon_s,lat_s,&ts);
      osd(lon_s,lat_s,ts);
      satellite(lon,lat);

      for (i=0;i<6000;i++)
	waitvsync();
      
    }
  
}
