#include <cmoc.h>
#include <coco.h>
#include "iss.h"
#include "font.h"

static byte *screenBuffer;

byte andTable[4] =
  {0x3F, 0xCF, 0xF3, 0xFC};
byte orTable[4][4] =
  {
    {0x00,0x00,0x00,0x00},
    {0x40,0x10,0x04,0x01},
    {0x80,0x20,0x08,0x02},
    {0xC0,0x30,0x0C,0x03}
  };

#define BUFFER_OFFSET(x,y) ((y << 5) + (x >> 2))
#define PIXEL_OFFSET(x) (x & 0x03)

void clear_bottom(void)
{
  memset(&screenBuffer[5120],0x00,1024);
}

void pset(int x, int y,unsigned char c)
{
  screenBuffer[BUFFER_OFFSET(x,y)] &= andTable[PIXEL_OFFSET(x)];
  screenBuffer[BUFFER_OFFSET(x,y)] |= orTable[c][PIXEL_OFFSET(x)];
}

void putc(int x, int y, char ch, char color)
{
  for (int i=0;i<8;i++)
    {
      char b = font[ch][i];
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
  
void set_screenbuffer(void)
{
  screenBuffer = (byte *) (((word) * (byte *) 0x00BC) << 8);
}

int main(void)
{
  initCoCoSupport();
  rgb();
  width(32);
  set_screenbuffer();

  pmode(3,screenBuffer);
  
  memcpy(screenBuffer,iss_gray,6144);
  for (int i=0;i<6144;i++)
    screenBuffer[i]^=0xFF;
  
  clear_bottom();
  
  puts(0,160,3,"  ISS POSITION  ");
  puts(0,168,2,"LON:     98.1420");
  puts(0,176,2,"LAT:    -12.5879");
  puts(0,184,1,"SAT DEC 30 14:04");

  puts(26,96,2,"\x1F");
  screen(1,1);

  while(1);

  return 0;
}
