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
#include "era.h"
#include "input.h"
#include "sp.h"

extern unsigned char buf[1024];
extern unsigned char net;

void era(char *s)
{
  if (s==NULL)
    {
      printf("URL: ");
      input(s);
    }

  if (s==NULL)
    return;
  
  memset(sp_payload,0,sizeof(sp_payload));
  
  sp_payload[0]=strlen(s)&0xff;
  sp_payload[1]=strlen(s)>>8;

  if (s!=NULL)
    strncpy((char *)&sp_payload[2],s,256);

  if (sp_control(net,'!'))
    printf("ERROR\n\n");
}
