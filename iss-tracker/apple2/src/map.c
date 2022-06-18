/**
 * ISS Tracker
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */
#include <apple2.h>
#include <6502.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void map(void)
{
  FILE *fp = fopen("MAP.HGR","rb");

  if (!fp)
    {
      perror("map open");
      cgetc();
      exit(1);
    }

  fread((void *)0x2000,sizeof(unsigned char),0x2000,fp);
  fclose(fp);
}
