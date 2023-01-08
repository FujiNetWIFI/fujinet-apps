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

unsigned char modem;

void in()
{
  unsigned short bw; // Bytes waiting
  unsigned short i;
  
  sp_status(modem,'S');

  bw = (unsigned short)sp_payload[0];

  if (bw==0)
    return;
  if (bw > sizeof(sp_payload))
    bw = sizeof(sp_payload);

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
