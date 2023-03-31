/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * Satellite display
 */

#include <c64.h>
#include <6502.h>
#include <string.h>

#define PPORT     ((unsigned char *)0x0001) // Processor port (RAM selection)
#define SPRD      ((unsigned char *)0xC000) // Sprite Data location
#define SPRDP     ((unsigned char *)0xD3F8) // Sprite data pointer relative to SPRD
#define SPRX      ((unsigned char *)0xD000) // X position (lower 8 bits)
#define SPRY      ((unsigned char *)0xD001) // Y position (lower 8 bits)
#define SPRXX     ((unsigned char *)0xD010) // X position (upper 1 bit)
#define SPRENABLE ((unsigned char *)0xD015) // Sprite enable bits
#define SPREX     ((unsigned char *)0xD017) // Sprite expansion bits for X
#define SPREY     ((unsigned char *)0xD01D) // Sprite expansion bits for Y
#define SPRCOLR   ((unsigned char *)0xD027) // Sprite color

#define X_CENTER  160
#define Y_CENTER  80

const unsigned char _satellite[63] = {
  0x20, 0x00, 0x00, /* ..X.|.... */
  0x50, 0x00, 0x00, /* .X.X|.... */
  0xA4, 0x00, 0x00, /* X.X.|.X.. */
  0x58, 0x00, 0x00, /* .X.X|X... */
  0x1A, 0x00, 0x00, /* ...X|X.X. */
  0x05, 0x00, 0x00, /* ....|.X.X */
  0x0A, 0x00, 0x00, /* ....|X.X. */
  0x04, 0x00, 0x00, /* ....|.X.. */
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};

/**
 * @brief Convert longitude to X coordinate
 * @param lon Longitude
 * @return X coordinate
 */
unsigned short lon_to_x(int lon)
{
  return X_CENTER + (unsigned short) ((lon << 3) / 9);
}

/**
 * @brief Convert longitude to X coordinate
 * @param lon Longitude
 * @return X coordinate
 */
unsigned char lat_to_y(int lat)
{
  return Y_CENTER - (unsigned char) ((lat << 3) / 9);
}

/**
 * @brief Display satellite at lon, lat
 * @param lon Integer longitude (-180, 180 deg)
 * @param lat Integer latitude  (-180, 180 deg)
 */
void satellite(int lon, int lat)
{
  unsigned short x = lon_to_x(lon);
  unsigned char  y = lat_to_y(lat);
  unsigned char  b = *PPORT;

  // Copy sprite to $C000
  memcpy(SPRD,_satellite,sizeof(_satellite));

  // Set sprite position, and expansions
  *SPRY=y;
  *SPRX=x&0xFF;
  *SPREX=1;
  *SPREY=1;

  // Set high bit if needed.
  if (x>255)
    *SPRXX=0x01;

  // Enable sprite
  *SPRENABLE=0x01;
  *SPRCOLR=0x07;

  // Swap in RAM, so that we can set the sprite pointer
  SEI();
  *PPORT = 0b00110000;
  *SPRDP = 0x00;
  *PPORT = b;
  CLI();
}
