/**
 * @brief   VT100 Terminal for Apple ][
 * @verbose Main entrypoint
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sp.h"
#include "vt100.h"
#include "screen.h"

#define MAX_SIZE 8192

unsigned char cpm;

unsigned char buf[MAX_SIZE];
unsigned short len=0;

#define SWITCH_80STORE (*(unsigned char *)0xC001)
#define SWITCH_RDMAINRAM (*(unsigned char *)0xC002)

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
    vt100(buf[j]);
}

void out()
{
  len=0;
  
  if (kbhit())
    {
      while (kbhit())
	sp_payload[len++]=cgetc();
      sp_write(cpm,len);
    }
}

void main(void)
{
  int i;
  
  SWITCH_80STORE=SWITCH_RDMAINRAM=false;
  
  sp_init();
  cpm = 3;
  
  screen_init();

  sp_payload[0]=1;
  sp_payload[1]=0;
  sp_payload[2]=0;
  
  sp_control(cpm,'B');

  for (i=0;i<2000;i++);
  
  while(1)
    {
      for (i=0;i<512;i++);
      in();
      out();
    }
}
