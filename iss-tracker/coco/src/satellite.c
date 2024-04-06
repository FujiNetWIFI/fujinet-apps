/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Satellite plot functions
 */

#include "pmode3.h"
#include "latlon.h"
#include "globals.h"

/**
 * @brief Satellite bitmap, 8x8 1bpp 
 */
static const unsigned char satellite_bitmap[8] = {
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
 * @brief plot satellite at pixel position X,Y
 * @param x Horizontal position (0-127)
 * @param y Vertical position (0-191)
 */
void satellite_plot(unsigned char _x, unsigned char _y)
{
    for (int y=0;y<8;y++)
    {
        for (int x=0;x<8;x++)
        {
            unsigned char b = (satellite_bitmap[y] << x) & 0x80;
            if (b)
                pxor(x+_x,y+_y);
        }
    }
}

/**
 * @brief plot satellite
 */
void satellite(void)
{
    satellite_plot(lon_to_x(lon_i),lat_to_y(lat_i));
}
