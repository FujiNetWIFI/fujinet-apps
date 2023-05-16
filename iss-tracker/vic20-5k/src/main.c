/**
 * @brief   ISS Tracker for 5 kilobyte VIC-20
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#include <vic20.h>
#include <stdbool.h>
#include <conio.h>
#include "video.h"
#include "fetch.h"
#include "osd.h"
#include "map.h"
#include "satellite.h"

char lon_s[16], lat_s[16];
int lon, lat;

void main(void)
{
  int i;
  
  while(true)
    {
      clrscr();
      print("\x8e"); // font switch to gfx/upper
      map();
      fetch(&lon,&lat,lon_s,lat_s);
      osd(lon_s,lat_s);
      satellite(lon,lat);
      
      for (i=0;i<6000;i++)
	waitvsync();
    }
}
