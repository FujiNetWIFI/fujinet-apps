/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief OSD output functions, uses text();
 */

#include <c64.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "text.h"

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 * @param ts 32-bit timestamp
 */
void osd(char *lon_s, char *lat_s, unsigned long ts)
{
  char tmp[80];

  graphics_gotoxy(0,21);
  sprintf(tmp,"      ** current iss position **\r\n");
  text(tmp,true);
  graphics_gotoxy(0,23);
  sprintf(tmp," lon: %-14s lat: %-14s",lon_s,lat_s);
  text(tmp,false);
  graphics_gotoxy(0,24);
  sprintf(tmp,"  as of: %s",ctime(&ts));
  text(tmp,false);
}
