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
#include <apple2.h>
#include "input.h"
#include "network.h"
#include "wait_for_connect.h"

#define CONNECTED 2
#define READ 4
#define NO_TRANSLATION 0

extern unsigned char buf[1024];

void get(char *s, char *t, char *u, char *v)
{
  char source[128], dest[24], type[2], aux[4];
  FILE *fp = NULL;
  unsigned long total=0;
  
  memset(source,0,128);
  memset(dest,0,24);
  
  if (s == NULL)
    {
      printf("SOURCE URL: ");
      s=source;
      input(s);

      if (s[0]==0x00)
	return;
    }

  if (t == NULL)
    {
      printf("DESTINATION FILE: ");
      t=dest;
      input(t);

      if (t[0]==0x00)
	return;
    }

  if (u == NULL)
    {
      printf("TYPE CODE $");
      u=type;
      input(u);
    }

  sscanf(u,"%02x\n",_filetype);

  if (v == NULL)
    {
      printf("AUX CODE $");
      v=aux;
      input(v);
    }

  sscanf(v,"%04x\n",_auxtype);

  network_open(s,READ,NO_TRANSLATION);

  if (!wait_for_connect())
    goto bye;

  fp = fopen(t,"wb");

  if (fp == NULL)
    {
      printf("COULD NOT OPEN DESTINATION FILE %s\n",t);
      goto bye;
    }

  while (1)
    {
      unsigned short l = network_read((char *)buf,sizeof(buf));

      if (fwrite(buf,sizeof(char),l,fp) != l)
	{
	  printf("COULD NOT WRITE TO DEST FILE. ABORTING.\n");
	  goto bye;
	}

      total += l;
      
      printf("%8lu BYTES TRANSFERRED.\r",total);
      
      if (!(network_statusbyte() & CONNECTED))
	{
	  l = network_read((char *)buf,sizeof(buf));
	  fwrite(buf,sizeof(char),l,fp);
	  printf("\n\nDONE.\n");
	  break;
	}
    }

 bye:
  network_close();
  fclose(fp);
}
