/**
 * map test program
 */

#include <6502.h>
#include <plus4.h>
#include <tgi.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include <stdbool.h>
#include "map.h"
#include "mapcm.h"
#include "maplm.h"
#include "text.h"

#define HIRES ((unsigned char *)0xC000)
#define LUMRM ((unsigned char *)0x0800)
#define COLRM ((unsigned char *)0x0C00)

extern void ted_hi_tgi[];

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

void satellite(unsigned short x, unsigned char y)
{
  unsigned char i,j;
  unsigned char b;
  
  for (i=0;i<8;i++)
    {
      b=_satellite[i];
      for (j=0;j<16;j+=2)
	{
	  if (b&0x80)
	    tgi_setcolor(1);
	  else
	    tgi_setcolor(0);
	  
	  tgi_setpixel(x+j,y+i);
	  tgi_setpixel(x+j+1,y+i);
	  b <<= 1;
	}
    }
}

void map(void)
{
  memcpy(COLRM,&map_cm[0],1000);
  memcpy(LUMRM,&map_lm[0],1000);
  memcpy(HIRES,&map_hir[0],8000);
}

void main(void)
{
  tgi_install(ted_hi_tgi);
  tgi_init();
  tgi_clear();
  POKE(0xFF19,0x46);
  map();
  satellite(152,96);
  memset(&LUMRM[21*40],0x0F,40);
  memset(&LUMRM[23*40+11],0x5F,11);
  memset(&LUMRM[23*40+32],0x5F,11);
  memset(&LUMRM[24*40+11],0x5F,29);
  graphics_gotoxy(0,21);
  text("     **** current iss position ****     ",true);
  graphics_gotoxy(0,23);
  text("longitude: -122.0124  latitude: -51.0123",false);
  text("    as of: wed mar 29 14:01:00 2023",false);
  cgetc();
  tgi_done();
}
