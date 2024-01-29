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

static char tmp[80];

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 * @param ts 32-bit timestamp
 */
void osd(char *lon_s, char *lat_s, unsigned long ts)
{
  int i;
	 
  graphics_gotoxy(0,21);
  text("       ** CURRENT ISS POSITION **       ",true);
  graphics_gotoxy(0,23);
  sprintf(tmp,"   LON: %-14s LAT: %-12s",lon_s,lat_s);
  text(tmp,false);
  graphics_gotoxy(0,24);
  sprintf(tmp,"  as of: %s UTC",ctime(&ts));
 
  // ctime() seems to have a bug where a garbage CR 'm' appears in the resulting string, replace it with a space 
  for (i = 0; i < strlen(tmp); i++) 
  {
    if (tmp[i] == 13) tmp[i] = 32;
  }
  
  text(tmp,false);
}
