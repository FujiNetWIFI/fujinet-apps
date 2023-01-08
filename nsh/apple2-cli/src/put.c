/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "network.h"
#include "wait_for_connect.h"

#define CONNECTED 2
#define WRITE 8
#define NO_TRANSLATION 0

void put(char *s, char *t)
{
  char source[24], dest[128];
  FILE *fp = NULL;
  unsigned long total=0;
  
  memset(source,0,24);
  memset(dest,0,128);
  
  if (s == NULL)
    {
      printf("SOURCE FILE: ");
      s=source;
      input(s);

      if (s[0]==0x00)
	return;
    }

  if (t == NULL)
    {
      printf("DESTINATION URL: ");
      t=dest;
      input(t);

      if (t[0]==0x00)
	return;
    }

  fp = fopen(s,"rb");

  if (fp == NULL)
    {
      printf("COULD NOT OPEN SOURCE FILE %s\n",t);
      goto bye;
    }

  
  network_open(t,WRITE,NO_TRANSLATION);

  /* if (!wait_for_connect2()) */
  /*   goto bye; */

  while (!feof(fp))
    {
      char buf[128];
      int l = fread(buf,sizeof(char),128,fp);
      
      network_write(buf,l);
      
      total += l;
      
      printf("\r%8lu BYTES TRANSFERRED.\r",total);
    }

  printf("\nDONE.\n");

 bye:
  network_close();
  fclose(fp);
}
