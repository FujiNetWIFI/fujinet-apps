/**
 * @brief   ISS Tracker for 5 kilobyte VIC-20
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#define X_CENTER 11
#define Y_CENTER 11

#include <conio.h>
#include "video.h"

void satellite(int lon, int lat)
{
  unsigned char x = X_CENTER + (unsigned char) ( lon >> 4 );
  unsigned char y = Y_CENTER - (unsigned char) ( lat >> 4 );

  gotoxy(x,y);
  print("\x12\x1c\xD1\x1f\x92");
}
