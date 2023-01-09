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
#include "sp.h"

#define MAX_SIZE 2000

unsigned char modem;

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short i;
  
  sp_status(modem,'S');

  bw  = sp_payload[0] & 0xFF;
  bw |= sp_payload[1] << 8;

  if (bw==0)
    return;
  if (bw > MAX_SIZE)
    bw = MAX_SIZE;

  memset(sp_payload,0,sizeof(sp_payload));
  
  sp_read(modem,bw);

  for (i=0;i<bw;i++)
    if (sp_payload[i]!='\r')
      putchar(sp_payload[i]);
}

void out()
{
  if (kbhit())
    {
      memset(sp_payload,0,sizeof(sp_payload));
      sp_payload[0]=cgetc();
      sp_write(modem,1);
    }
}

void main(void)
{
  sp_init();
  modem = sp_find_modem();

  clrscr();
  
  printf("MODEM DEVICE IS %u\nTERMINAL READY.\n\n",modem);

  while(1)
    {
      cursor(1);
      in();
      out();
    }
}
