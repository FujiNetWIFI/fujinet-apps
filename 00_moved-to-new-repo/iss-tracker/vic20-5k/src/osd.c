/**
 * @brief   ISS Tracker for 5 kilobyte VIC-20
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include "video.h"
#include "osd.h"

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 */
void osd(char *lon_s, char *lat_s)
{
  gotoxy(0,19);
  print("\x12""\x05""current  iss  position\x92");
  gotoxy(0,21);
  print("lat: ");
  print(lat_s);
  print("\r\n");
  print("lon: ");
  print(lon_s);
}
