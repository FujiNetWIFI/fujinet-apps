/**
 * #FujiNet CP/M terminal for Apple2
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2enh.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "sp.h"

unsigned char cpm;

#define MAX_SIZE 8192

unsigned char buf[MAX_SIZE];

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short b,i,j;
  
  sp_status(cpm,'S');

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
      
      sp_read(cpm,b);
      
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
  if (kbhit())
    {
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=1;
      sp_payload[1]=0;
      sp_payload[2]=cgetc();
      sp_control(cpm,'W');
    }
}

void main(void)
{  
  unsigned short i;

  videomode(VIDEOMODE_80x24);
  sp_init();
  cpm = sp_find_cpm();

  clrscr();
  
  printf("CP/M DEVICE IS %u\n\n",cpm);

  printf("STARTING CP/M, PLEASE WAIT...\r\n");

  sp_control(cpm,'B'); // Boot.
  
  while(1)
    {
      cursor(1);
      in();
      for (i=0;i<8192;i++);
      out();
    }
}
