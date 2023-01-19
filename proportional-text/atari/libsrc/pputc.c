/**
 * Proportional Font text output library for Atari
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <tgi.h>

/** 
 * @var _pcx Cursor X position (pcursor.c)
 */
extern unsigned short _pcx;

/** 
 * @var _pcx Cursor Y position (pcursor.c)
 */
extern unsigned char _pcy;

/**
 * @var font The Font (font.c)
 */
extern const unsigned char font[];

/**
 * @brief Output a proportional font character at cursor
 * @param c Character to output
 */
unsigned char pputc(const char c)
{
  // map ASCII to appropriate character
  unsigned short o;  // offset 
  unsigned char w;   // width
  unsigned char i;   // row
  unsigned short j;  // column
  
  o = (c << 3) + c; // mul 9
  
  w = font[o++]; // First byte is width, also increment past it.

  for (i=0;i<8;i++)
    {
      unsigned char b = font[o+i]; // the whole row byte
      
      for (j=0;j<w;j++)
	{
	  if ((_pcx+j) > 319)
	    continue;
	  
	  if (((b & 0x80) == 0x80) && ((_pcx+j) <= 0xff))
	    tgi_setpixel(_pcx+j,_pcy+i);
	  
	  b <<= 1; // shift next bit to leftmost.
	}
    }
  
  return w;
}
