/**
 * map test for vic20 tgi
 */

#include <vic20.h>
#include <tgi.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include <stdbool.h>
#include "map.h"

#define SCRN  ((unsigned char *)0x1100)
#define IHROM ((unsigned char *)0x8800)
#define CHROM ((unsigned char *)0x8C00)
#define COLR  ((unsigned char *)0x9400)

#define CHARSIZE 8

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

int _cursor=0;

/**
 * @brief Convert PETSCII to screen code
 * @param c The character to convert
 */
char _scrncode(char c)
{
  if (c == 0xff)
    c = 0x5e; // pi
  else if (c <= 0xFE && c >= 0xE0)
    c -= 0x80;
  else if (c <= 0xDF && c >= 0xC0)
    c -= 0x80;
  else if (c <= 0xBF && c >= 0xA0)
    c -= 0x40;
  else if (c <= 0x9F && c >= 0x80)
    c += 0x40;
  else if (c <= 0x7F && c >= 0x60)
    c -= 0x20;
  else if (c <= 0x5F && c >= 0x40)
    c -= 0x40;
  else if (c <= 0x3F && c >= 0x20)
    c = c;
  else if (c <= 0x1F && c >= 0x01)
    c += 0x80;
  
  return c;
}

void _text(char _c, bool i)
{
  unsigned char *sp,*dp;
  
  _c = _scrncode(_c);

  if (i)
    sp = IHROM + (_c << 3);
  else
    sp = CHROM + (_c << 3);

  dp = SCRN + _cursor;

  memcpy(dp,sp,CHARSIZE);

  _cursor += 192; 
}

/**
 * @brief place string s at cursor
 * @param s The string of PETSCII characters to type
 * @param i Inverse? (true or false)
 */
void text(const char *s, bool i)
{
  char c;
  
  while (c = *s++)
    _text(c,i);
}

void graphics_gotoxy(unsigned char x, unsigned char y)
{
  _cursor = (y*8) + (x*192);
}

void drawmap(void)
{
  memcpy(SCRN,mapdata,sizeof(mapdata));
  memset(&COLR[200],0x21,40);
}

void satellite(unsigned char x, unsigned char y)
{
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

void main(void)
{
  const unsigned char pal[2] = {6,5};
  
  tgi_install(vic20_hi_tgi);
  tgi_init();
  tgi_setpalette(pal);
  POKE(0x900F,0x66);
  tgi_clear();
  drawmap();
  satellite(0,0);
  POKE(0x9400,0x01);
  POKE(0x9401,0x01);
  graphics_gotoxy(0,20);
  text("CURRENT ISS POSITION",true);
  graphics_gotoxy(0,22);
  text("LAT: -123.2209",false);
  graphics_gotoxy(0,23);
  text("LON: -51.0345",false);
  cgetc();
  tgi_done();
}
