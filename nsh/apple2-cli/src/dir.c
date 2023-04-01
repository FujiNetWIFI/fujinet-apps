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

#define DIRECTORY 6
#define LONG_DIRECTORY 128
#define NERR_EOF 136
#define ATASCII_EOL 0x9B

extern unsigned char buf[8192];

void dir(char *s)
{
  char default_path[3] = "N:";
  unsigned char err;
  unsigned char nerr=1;
  unsigned short bw;
  unsigned char *p;

  if (!s)
    s=&default_path[0];

  if ((err = network_open(s,DIRECTORY,LONG_DIRECTORY)) != SP_ERR_NOERROR)
    {
      printf("OPEN ERROR: %u\n\n",err);
      network_close();
      return;
    }

  do
    {
      memset(buf,0,sizeof(buf));
  
      if (err = network_status(&bw,NULL,&nerr))
	{
	  printf("STATUS ERROR: %u\n\n",nerr);
	  network_close();
	  return;
	}
      
      if ((err = network_read(buf,bw)) != SP_ERR_NOERROR)
	{
	  printf("READ ERROR: %u\n\n",err);
	  network_close();
	  return;
	}

      p = &buf[0];

      while (bw--)
	{
	  if (*p==0x9B) // convert EOL to Line feed for putchar.
	    *p=0x0A;

	  putchar(*p++);
	}
    } while (nerr != 136);

  if (err = network_status(&bw,NULL,&nerr))
    {
      printf("STATUS ERROR: %u\n\n",nerr);
      network_close();
      return;
    }

  if ((err = network_read(buf,bw)) != SP_ERR_NOERROR)
    {
      printf("READ ERROR: %u\n\n",err);
      network_close();
      return;
    }
  
  p = &buf[0];
  
  while (bw--)
    {
      if (*p==0x9B) // convert EOL to Line feed for putchar.
	*p=0x0A;
      
      putchar(*p++);
    }
  
}
