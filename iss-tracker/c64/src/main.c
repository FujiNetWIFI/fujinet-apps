/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief main()
 */

#include <c64.h>
#include <tgi.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "map.h"
#include "fetch.h"
#include "osd.h"
#include "satellite.h"

unsigned long ts;

char lon_s[16], lat_s[16];
int lon, lat;

void main(void)
{
  tgi_install(c64_hi_tgi);
  tgi_init();
  tgi_clear();
  
  map();
  
  while(true)
  {
    fetch(&lon,&lat,lon_s,lat_s,&ts);
    osd(lon_s,lat_s,ts);
    satellite(lon,lat);

    sleep(10);
  }
}
