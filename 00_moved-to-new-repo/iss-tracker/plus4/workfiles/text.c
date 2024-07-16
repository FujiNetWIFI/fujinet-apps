/**
 * @brief Quick and Dirty text routines for C64 bitmap mode
 * @author Thomas Cherryhomes
 * @email  thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <6502.h>
#include <plus4.h>
#include <peekpoke.h>
#include <string.h>
#include <stdbool.h>
#include "text.h"
#include "chargen.h"

/**
 * @brief Location of bitmap display as set up by TGI
 */
#define SCRN ((unsigned char *)0xC000)

#define FF12 ((unsigned char *)0xFF12)

/**
 * @brief Size of each character in ROM in lines
 */
#define CHARSIZE 8

/**
 * @brief the cursor position in memory
 */
static unsigned short _cursor=0;

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

/**
 * @brief place character c at cursor; advance cursor
 * @param c The PETSCII character to place
 */
void _text(char c, bool i)
{
  unsigned char *sp,*dp;
  unsigned char b = *FF12;
  
  c = _scrncode(c);

  if (i)
    sp = &chargen[1024] + (c << 3);
  else
    sp = &chargen[0] + (c << 3);
  dp = SCRN + _cursor;

  *FF12 |= 0x08; // fetch from ROM
  memcpy(dp,sp,CHARSIZE);
  _cursor += 8;
  *FF12 = b;
}

/**
 * @brief place string s at cursor
 * @param s The string of PETSCII characters to type
 */
void text(const char *s, bool i)
{
  char c;
  
  while (c = *s++)
    _text(c,i);
}

/**
 * @brief place cursor at x,y
 * @param x the horizontal offset of line (0-39)
 * @param y the line (0-24)
 */
void graphics_gotoxy(unsigned char x, unsigned char y)
{
  _cursor = (y * 320) + (x * 8);
}
