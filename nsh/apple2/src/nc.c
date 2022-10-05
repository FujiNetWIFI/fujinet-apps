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
#include "adamnet_read.h"
#include "adamnet_write.h"
#include "network.h"
#include "wait_for_connect.h"
#include "input.h"

extern unsigned char response[1024];
unsigned char txbuf[64];

#define READWRITE 12
#define CONNECTED 2
#define NO_TRANSLATION 0

void in(void)
{
  unsigned short l = network_read(response, sizeof(response));
  
  if (l > 0)
    for (unsigned short i=0;i<l;i++)
      cputc(response[i]);
}

void out(void)
{
  char k=0;
  
  if (kbhit())
    {
      txbuf[0]=cgetc();
      network_write(txbuf,1); // send one char.
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

  if (network_open(s,READWRITE,NO_TRANSLATION) != 0x80) // Testing to see if we're nulling the string.
    {
      printf("COULD NOT OPEN SOCKET\n");
      goto bye;
    }

  if (!wait_for_connect2())
    {
      printf("COULD NOT CONNECT\n");
      goto bye;
    }

  cprintf("CONNECTED.\n");

  while(1)
    {
      in();
      out();

      if (sc==0)
	if (check_for_disconnect())
	  {
	    printf("DISCONNECTED\n");
	    break;
	  }

      sc++;
    }
  
 bye:
  network_close();
  
}
