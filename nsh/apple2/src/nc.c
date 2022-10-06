/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "dir.h"
#include "network.h"
#include "wait_for_connect.h"
#include "input.h"
#include "sp.h"

extern unsigned char buf[1024];
extern unsigned char net;
unsigned char txbuf[64];

#define READWRITE 12
#define CONNECTED 2
#define NO_TRANSLATION 0

void in(void)
{
  unsigned short i;
  unsigned short l;

  sp_status(net,'S');

  l=(unsigned short)sp_payload[0];

  if (l>1024)
    l=1024;
  
  if (l > 0)
    {
      sp_read(net,l);
      
      memcpy(buf,sp_payload,l);

      for (i=0;i<l;i++)
	putchar(buf[i]);
    }
}

void out(void)
{
  char k=0;
  
  if (kbhit())
    {
      txbuf[0]=cgetc();
      network_write((char *)txbuf,1); // send one char.
    }
}

bool check_for_disconnect(void)
{
  return !(network_statusbyte() & CONNECTED);
}

void nc(char *s)
{
  char url[128];
  unsigned char sc=0; // status check counter, only 1 of every 256 ticks.
  
  memset(url,0,sizeof(url));
  
  if (s==NULL)
    {
      printf("URL: ");
      s=url;
      input(s);
    }

  network_open(s,READWRITE,NO_TRANSLATION); 

  cprintf("CONNECTED.\n");

  while(1)
    {
      in();
      out();

      /* if (sc==0) */
	/* if (check_for_disconnect()) */
	/*   { */
	/*     printf("DISCONNECTED\n"); */
	/*     break; */
	/*   } */

      sc++;
    }
  
 bye:
  network_close();
  
}
