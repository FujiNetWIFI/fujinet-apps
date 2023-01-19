/**
 * #FujiNet Modem terminal for Apple2
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2enh.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include "sp.h"

#define MAX_SIZE 8192

unsigned char modem;

unsigned char buf[MAX_SIZE];
unsigned short len=0;

const unsigned short row[24]= {
			       0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700,
			       0x0780, 0x0428, 0x04A8, 0x0528, 0x05A8, 0x0628, 0x06A8, 0x0728, 0x07A8,
			       0x0450, 0x04D0, 0x0550, 0x05D0, 0x0650, 0x06D0, 0x0750, 0x07D0
};

void updateCursor(void)
{
  unsigned char x = wherex();
  unsigned char y = wherey();
  unsigned char b = PEEK(row[y]+x) & 0x7F;
  
  POKE(row[y]+x,b);
}

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short b,i,j;
  
  sp_status(modem,'S');

  bw  = sp_payload[0] & 0xFF;
  bw |= sp_payload[1] << 8;

  if (bw==0)
    return;
  if (bw > MAX_SIZE)
    bw = MAX_SIZE;

  memset(sp_payload,0,sizeof(sp_payload));

  i=0;
  
  while (bw > 0)
    {
      if (bw > 512)
	b = 512;
      else
	b = bw;
      
      sp_read(modem,b);
      
      memcpy(&buf[i],&sp_payload[0],b);

      i += b;
      bw -= b;
    }
  
  for (j=0;j<i;j++)
    if (buf[j]!='\r')
      {
	putchar(buf[j]);
	updateCursor();
      }
}

void out()
{
  len=0;
  
  if (kbhit())
    {
      while (kbhit())
	sp_payload[len++]=cgetc();
      sp_write(modem,len);
    }
}

void main(void)
{
  videomode(VIDEOMODE_80x24);
  sp_init();
  modem = sp_find_modem();

  clrscr();
  
  printf("MODEM DEVICE IS %u\nTERMINAL READY.\n\n",modem);

  while(1)
    {
      in();
      out();
    }
}
