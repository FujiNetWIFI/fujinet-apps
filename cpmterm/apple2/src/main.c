/**
 * #FujiNet CPM terminal for Apple2
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

unsigned char CPM;

unsigned char buf[MAX_SIZE];
unsigned short len=0;

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short b,i,j;

  for (i=0;i<2000;i++);
  
  sp_status(CPM,'S');

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
      
      sp_read(CPM,b);
      
      memcpy(&buf[i],&sp_payload[0],b);

      i += b;
      bw -= b;
    }
  
  for (j=0;j<i;j++)
    if (buf[j]!='\r')
      putchar(buf[j]);
}

void out()
{
  len=0;
  
  if (kbhit())
    {
      while (kbhit())
	sp_payload[len++]=cgetc();
      sp_write(CPM,len);
    }
}

void main(void)
{
  int i;
  
  videomode(VIDEOMODE_80x24);
  sp_init();
  CPM = sp_find_cpm();

  clrscr();

  sp_control(CPM,'B');

  for (i=0;i<8000;i++);
  
  printf("CPM DEVICE IS %u\nTERMINAL READY.\n\n",CPM);

  while(1)
    {
      in();
      out();
    }
}
