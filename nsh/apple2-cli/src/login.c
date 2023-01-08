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
#include <stdbool.h>
#include "mkdir.h"
#include "input.h"
#include "sp.h"

extern unsigned char net;
extern bool _input_pw;

char u[128], p[128];

void login(char *s, char *t)
{  
  if (s==NULL)
    {
      s=u;
      printf("USERNAME: ");
      input(s);
    }

  if (t==NULL)
    {
      _input_pw=true;
      t=p;
      printf("PASSWORD: ");
      input(t);
      _input_pw=false;
    }

  sp_payload[0]=128;
  sp_payload[1]=0;
  
  strncpy((char *)&sp_payload[2],s,127);
  sp_control(net,0xFD);

  strncpy((char *)&sp_payload[2],t,127);
  sp_control(net,0xFE);

  printf("\n");
}
