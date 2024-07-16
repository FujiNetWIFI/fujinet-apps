/**
 * @brief   PMODE 3 Graphic helper Functions
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Plot 2bpp pixels and print text functions.
 */

#include <cmoc.h>
#include <coco.h>

/**
 * @brief internal pointer for screen buffer
 */
static byte *_screenBuffer;

/**
 * @brief font to use for putc
 */
extern const unsigned char _font[128][8]; 

/**
 * AND table for 2 bits per pixel
 */
byte andTable[4] =
  {0x3F, 0xCF, 0xF3, 0xFC};

/**
 * OR table for 2 bits per pixel, 
 * for each of the 4 possible colors
 */
byte orTable[4][4] =
  {
    {0x00,0x00,0x00,0x00},
    {0x40,0x10,0x04,0x01},
    {0x80,0x20,0x08,0x02},
    {0xC0,0x30,0x0C,0x03}
  };

/**
 * @brief macro to calculate byte position of pixel
 */
#define BUFFER_OFFSET(x,y) ((y << 5) + (x >> 2))

/**
 * @brief macro to calculate bit offset of pixel in byte
 */
#define PIXEL_OFFSET(x) (x & 0x03)

/**
 * @brief copy whole screen bitmap to screen buffer
 * @param b pointer to 6144 byte bitmap
 */
void pmode3_memcpy(const byte *b)
{
    memcpy(_screenBuffer,b,6144);
}

/**
 * @brief XOR all screen pixels, inverting them 
 */
void pmode3_xor(void)
{
    for (int i=0;i<6144;i++)
        _screenBuffer[i]^=0xFF;
}

/**
 * @brief Place pixel on PMODE3 display at x,y with color c 
 * @param x Horizontal Position (0-127) 
 * @param y Vertical Position (0-191)
 * @param c Color (0-3)
 */
void pset(int x, int y,unsigned char c)
{
  _screenBuffer[BUFFER_OFFSET(x,y)] &= andTable[PIXEL_OFFSET(x)];
  _screenBuffer[BUFFER_OFFSET(x,y)] |= orTable[c][PIXEL_OFFSET(x)];
}

/**
 * @brief XOR pixel on PMODE3 display at x,y 
 * @param x Horizontal Position (0-127) 
 * @param y Vertical Position (0-191)
 * @param c Color (0-3)
 * @verbose borrows fourth entry from orTable which inverts both bits.
 */
void pxor(int x, int y)
{
    _screenBuffer[BUFFER_OFFSET(x,y)] ^= orTable[3][PIXEL_OFFSET(x)];
}

/**
 * @brief clear from lines 160-192 
 */
void clear_bottom(void)
{
  memset(&_screenBuffer[5120],0x00,1024);
}

/**
 * @brief place character from font at x,y with color
 * @param x Horizontal position (0-127) 
 * @param y Vertical position (0-191)
 * @param color to use (0-3)
 */
void putc(int x, int y, char ch, char color)
{
  for (int i=0;i<8;i++)
    {
      char b = _font[ch][i];
      for (int j=0;j<8;j++)
	{
	  if (b < 0)
	    pset(x,y,color);

	  b <<= 1;
	  
	  x++;
	}
      y++;
      x -= 8;
    }
}

/**
 * @brief print s string at x,y with color 
 * @param x Horizontal Position (0-127)
 * @param y Vertical Position (0-191)
 * @param color to use (0-3)
 * @param s NULL terminated string to output. 
*/
void puts(int x, int y, char color, const char *s)
{
  while (*s)
    {
      putc(x,y,*s++,color);
      x += 8;

      if (x > 128)
	{
	  x=0;
	  y += 8;
	}
    }
}

/**
 * @brief Set and display PMODE3 (128x192x4c)
 */
void pmode3(void)
{
    initCoCoSupport();
    rgb();
    width(32);
    _screenBuffer = (byte *) (((word) * (byte *) 0x00BC) << 8);
    pmode(3,_screenBuffer);
    screen(1,1);
}
