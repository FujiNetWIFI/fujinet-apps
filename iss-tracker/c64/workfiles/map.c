/**
 * map test program
 */

#include <6502.h>
#include <c64.h>
#include <tgi.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include "map.h"
#include "text.h"

#define PPORT ((unsigned char *)0x0001)
#define HIRES ((unsigned char *)0xE000)
#define COLRM ((unsigned char *)0xD000)
#define CHROM ((unsigned char *)0xD000)

#define SPRD      ((unsigned char *)0x0340)
#define SPRDP     ((unsigned char *)0xD3F8)
#define SPRX      ((unsigned char *)0xD000)
#define SPRY      ((unsigned char *)0xD001)
#define SPRXX     ((unsigned char *)0xD010)
#define SPRENABLE ((unsigned char *)0xD015)
#define SPRCOLR   ((unsigned char *)0xD027)

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

void satellite(unsigned short x, unsigned char y)
{
  POKE(0xdd00,0);POKE(0xd018,0x48);
  memcpy(SPRD,_satellite,sizeof(_satellite));
  
  *SPRY=y;
  *SPRX=x&0xFF;

  if (x>255)
    *SPRXX=0x01;

  *SPRENABLE=0x01;

  *SPRCOLR=0x07;
}

void map(void)
{
  unsigned char b = *PPORT;

  SEI();
  *PPORT = 0b00110000;
  POKE(0xdd00,0);POKE(0xd018,0x48);
  POKE(0xD3F8,13);
  memcpy(HIRES,&map_hir[0],8000);
  memcpy(COLRM,&map_hir[8000],1000);
  *PPORT = b;
  CLI();
}

void main(void)
{
  tgi_install(c64_hi_tgi);
  tgi_init();
  tgi_clear();
  map();
  satellite(64,64);
  graphics_gotoxy(0,21);
  graphics_gotoxy(0,21);
  text("     **** current iss position ****     ",true);
  graphics_gotoxy(0,23);
  text("longitude: -122.0124  latitude: -51.0123",false);
  text("    as of: wed mar 29 14:01:00 2023",false);
  cgetc();
  tgi_done();
}
