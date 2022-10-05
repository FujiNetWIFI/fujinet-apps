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
#include "adamnet_read.h"
#include "adamnet_write.h"
#include "network.h"
#include "wait_for_connect.h"

extern unsigned char response[1024];

#define DIRECTORY 6
#define LONG_DIRECTORY 128
#define CONNECTED 2
#define ATASCII_EOL 0x9B

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
      char buf[1024];
      unsigned short l = network_read(buf,sizeof(buf));
      
      for (int i=0;i<l;i++)
	putchar(buf[i]==ATASCII_EOL ? 0x0A : buf[i]);
      
    } while (network_statusbyte() & CONNECTED);
  
  
 bye:
  network_close();
}
