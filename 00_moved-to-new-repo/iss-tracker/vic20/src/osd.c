/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief OSD output functions, uses text();
 */

#include <vic20.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define SCRN  ((unsigned char *)0x1100)
#define IHROM ((unsigned char *)0x8800)
#define CHROM ((unsigned char *)0x8C00)
#define COLR  ((unsigned char *)0x9400)
#define CHARSIZE 8

static char tmp[80];

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

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 * @param ts 32-bit timestamp
 */
void osd(char *lon_s, char *lat_s, unsigned long ts)
{
  graphics_gotoxy(0,20);
  text("CURRENT ISS POSITION",true);
  graphics_gotoxy(0,22);
  sprintf(tmp,"LAT: %s",lat_s);
  text(tmp,false);
  graphics_gotoxy(0,23);
  sprintf(tmp,"LON: %s",lon_s);
  text(tmp,false);
}
