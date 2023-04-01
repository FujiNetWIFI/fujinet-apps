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
#include "mkdir.h"
#include "input.h"
#include "sp.h"

extern unsigned char buf[8192];
extern unsigned char net;

void mkdir(char *s)
{
  if (s==NULL)
    {
      s=(char *)buf;
      printf("URL: ");
      input(s);
    }

  if (s==NULL)
    return;

  sp_payload[0]=0x02;
  sp_payload[1]=0x01;
  
  if (s!=NULL)
    strncpy((char *)&sp_payload[2],s,256);
  
  if (sp_control(net,'*'))
    printf("ERROR\n");
}
