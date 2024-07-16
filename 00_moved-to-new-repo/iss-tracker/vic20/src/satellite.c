/**
 * #FujiNet ISS tracker for VIC20
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * Satellite display
 */

#include <vic20.h>
#include <6502.h>
#include <string.h>
#include <tgi.h>

#define COLOR       ((unsigned char*)0x9400)

#define X_CENTER  79
#define Y_CENTER  39

const unsigned char _satellite[8] = {
  0x20, /* ..X.|.... */
  0x50, /* .X.X|.... */
  0xA4, /* X.X.|.X.. */
  0x58, /* .X.X|X... */
  0x1A, /* ...X|X.X. */
  0x05, /* ....|.X.X */
  0x0A, /* ....|X.X. */
  0x04, /* ....|.X.. */
};

/**
 * @brief Convert longitude to X coordinate
 * @param lon Longitude
 * @return X coordinate
 */
unsigned short lon_to_x(int lon)
{
  return X_CENTER + (unsigned char) ((lon << 2) / 9);
}

/**
 * @brief Convert longitude to X coordinate
 * @param lon Longitude
 * @return X coordinate
 */
unsigned char lat_to_y(int lat)
{
  return Y_CENTER - (unsigned char) ((lat << 2) / 9);
}

/**
 * @brief Display satellite at lon, lat
 * @param lon Integer longitude (-180, 180 deg)
 * @param lat Integer latitude  (-180, 180 deg)
 */
void satellite(int lon, int lat)
{
  unsigned short o;
  unsigned short x = lon_to_x(lon);
  unsigned char  y = lat_to_y(lat);
  unsigned char i,j;
  unsigned char b;

  for (i=0;i<8;i++)
    {
      b=_satellite[i];
      for (j=0;j<16;j+=2)
	{
	  if (b&0x80)
	    tgi_setcolor(0);
	  else
	    tgi_setcolor(1);
	  tgi_setpixel(x+j,y+i);
	  tgi_setpixel(x+j+1,y+i);
	  b <<= 1;
	}
    }

}
