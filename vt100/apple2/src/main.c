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

unsigned char cpm, modem, dev;

unsigned char buf[MAX_SIZE];
unsigned short len=0;

#define SWITCH_40STORE (*(unsigned char *)0xC000)
#define SWITCH_80STORE (*(unsigned char *)0xC001)
#define SWITCH_RDMAINRAM (*(unsigned char *)0xC002)

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short b,i,j;

  screen_unuse();
  
  if (sp_status(dev,'S')==SP_ERR_IOERROR)
    return;

  bw  = sp_payload[0] & 0xFF;
  bw |= sp_payload[1] << 8;

  if (bw==0)
    return;
  else if (bw==65535)
    exit(0);
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

      if (sp_read(dev,b) == SP_ERR_IOERROR)
	return;
      
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

  screen_unuse();
  if (kbhit())
    {
      while (kbhit())
	sp_payload[len++]=cgetc();
      sp_write(dev,len);
    }
}

void main(void)
{
  int i;
  char c;

  SWITCH_40STORE=SWITCH_RDMAINRAM=true;

  sp_init();
  cpm = sp_find_cpm();
  modem = sp_find_modem();

  screen_init();

  screen_puts("#FUJINET VT100 - C)P/M, or M)ODEM? ");
  while (true)
    {
      c=cgetc();
      if (c == 'C' || c == 'c')
	{
	  screen_putc('C');
	  dev = cpm;
	  sp_payload[0]=1;
	  sp_payload[1]=0;
	  sp_payload[2]=0;
	  sp_control(dev,'B');
	  for (i=0;i<16384;i++);
	  break;
	}
      else if (c == 'M' || c == 'm')
	{
	  screen_putc('M');
	  dev = modem;
	  break;
	}      
    }
  
  screen_cr();
  screen_lf();
  screen_cr();
  screen_lf();
      
  while(1)
    {
      in();
      out();
    }
}
