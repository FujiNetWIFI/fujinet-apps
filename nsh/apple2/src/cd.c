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
#include "cd.h"
#include "sp.h"

extern unsigned char net;

void cd(char *s)
{
  memset(sp_payload,0,sizeof(sp_payload));
  
  if (s!=NULL)
    strncpy((char *)&sp_payload[2],s,256);

  sp_payload[0]=0x00;
  sp_payload[1]=0x01;
  sp_control(net,',');
}
