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
#include "dir.h"
#include "sp.h"
#include "network.h"
#include "wait_for_connect.h"

#define DIRECTORY 6
#define LONG_DIRECTORY 128
#define CONNECTED 2
#define ATASCII_EOL 0x9B

extern unsigned char buf[1024];

void dir(char *s)
{
  if (s == NULL)
    network_open("N:",DIRECTORY,LONG_DIRECTORY);
  else
    network_open(s,DIRECTORY,LONG_DIRECTORY);

  if (!wait_for_connect())
    goto bye;

  do
    {
      unsigned short l = network_read((char *)buf,sizeof(buf));
      int i;
      
      for (i=0;i<l;i++)
	putchar(buf[i]==ATASCII_EOL ? 0x0A : buf[i]);
      
    } while (network_statusbyte() & CONNECTED);
  
  
 bye:
  network_close();
}
