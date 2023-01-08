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

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short i;
  
  sp_status(cpm,'S');

  bw = (unsigned short)sp_payload[0];

  if (bw==0)
    return;
  if (bw > sizeof(sp_payload))
    bw = sizeof(sp_payload);

  memset(sp_payload,0,sizeof(sp_payload));
  
  sp_read(cpm,bw);

  for (i=0;i<bw;i++)
    if (sp_payload[i]!='\r')
      putchar(sp_payload[i]);
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
