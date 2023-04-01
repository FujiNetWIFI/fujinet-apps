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
#include <apple2_filetype.h>
#include <conio.h>
#include "sp.h"
#include "input.h"
#include "network.h"

#define CONNECTED 2
#define MODE_READ 4
#define NO_TRANSLATION 0

extern unsigned char buf[8192];

static char source[256];
static char dest[24];
static char tmp[5];

#define NERR_EOF 136

void get(char *s, char *t, char *u, char *v)
{
  unsigned char err=0, nerr=1;
  FILE *fp;
  unsigned short bw=0;
  unsigned char len;
  unsigned long bt=0;

  memset(buf,0,sizeof(buf));
  
  if (!s)
    {
      printf("URL: ");
      input(source);

      if (source[0]==0x00)
	return;
    }
  else
    strcpy(source,s);

  if (!t)
    {
      printf("FILENAME: ");
      input(dest);

      if (dest[0]==0x00)
	return;
    }
  else
    strcpy(dest,t);

  if (!u)
    {
      printf("TYPE (HEX) $");
      input(tmp);

      if (tmp[0]==0x00)
	return;
    }
  else
    strcpy(tmp,u);

  sscanf(tmp,"%02X",_filetype);

  if (!v)
    {
      printf("AUX (HEX) $");
      input(tmp);

      if (tmp[0]==0x00)
	return;
    }
  else
    strcpy(tmp,v);

  sscanf(tmp,"%04X",_auxtype);

  if ((err = network_open(source,MODE_READ,NO_TRANSLATION)) != SP_ERR_NOERROR)
    {
      printf("OPEN ERROR: %u\n\n",err);
      network_close();
      return;
    }

  fp = fopen(dest,"wb");

  if (!fp)
    {
      printf("DESTINATION OPEN ERROR: %u\n\n",ferror(fp));
      network_close();
      return;
    }
  
  while (nerr != NERR_EOF)
    {      
      if ((err = network_status(&bw,NULL,&nerr)) != SP_ERR_NOERROR)
	{
	  printf("STATUS ERROR: %u\n\n",err);
	  network_close();
	  return;
	}

      if (bw>sizeof(buf))
	bw==sizeof(buf);

      if ((err = network_read(&buf[0],bw)) != SP_ERR_NOERROR)
	{
	  printf("READ ERROR: %u\n\n",err);
	  network_close();
	  return;
	}

      fwrite(&buf[0],sizeof(unsigned char),bw,fp);
      
      bt += bw;
      
      cprintf("%8lu BYTES TRANSFERRED\r",bt);
    }

  if ((err = network_status(&bw,NULL,&nerr)) != SP_ERR_NOERROR)
    {
      printf("STATUS ERROR: %u\n\n",err);
      network_close();
      return;
    }
  
  if (bw>sizeof(buf))
    bw==sizeof(buf);
  
  if ((err = network_read(&buf[0],bw)) != SP_ERR_NOERROR)
    {
      printf("READ ERROR: %u\n\n",err);
      network_close();
      return;
    }
  
  fwrite(&buf[0],sizeof(unsigned char),bw,fp);
    
  bt += bw;
  
  cprintf("%8lu BYTES TRANSFERRED\r",bt);
  
  printf("\x07\nDONE\n");

  fclose(fp);
  network_close();
}
