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
           short bw; // Bytes waiting
  unsigned short b,i,j,err;

  for(j=0; j<2000;j++);
  
  screen_unuse();

  bw = sp_bytes_waiting(cpm);

  if (bw<=0)
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

    if ((err=sp_read(cpm, b)) != SP_ERR_OK)
    {
      printf("\r\nREAD ERROR $%x\r\n", err);
      break;
    } else
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

  while (kbhit())
    sp_payload[len++]=cgetc();  

  if (len > 0)
    sp_write(cpm, len);

}

void main(void)
{
  int i,bw;
  int booted = 0;
  
  SWITCH_80STORE=SWITCH_RDMAINRAM=false;
  
  sp_init();
  
  screen_init();

  cpm = sp_find_cpm();

  sp_status(cpm,'B');

  screen_puts("FujiNet CP/M.\r\n");
  
  if (sp_payload[0]==1)
  {
      screen_puts("CP/M Device Found. Booting CP/M.\r\n\r\n");
      dev = cpm;
      sp_payload[0]=1;
      sp_payload[1]=0;
      sp_payload[2]=0;
      sp_control(dev,'B');
      for (i=0;i<16384;i++);
  }
  else
  {
      screen_puts("CP/M Not Found. Halted.");
      while(1);
  }
  
  screen_cr();
  screen_lf();
  screen_cr();
  screen_lf();
  
  sp_payload[0]=1;
  sp_payload[1]=0;
  sp_payload[2]=0;
  
  while (1)
  {
      in();
      out();
  }
}
