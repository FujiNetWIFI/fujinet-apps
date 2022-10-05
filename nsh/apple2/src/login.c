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
#include "adamnet_read.h"
#include "adamnet_write.h"
#include "input.h"

extern unsigned char response[1024];
extern bool _input_pw;

void login(char *s, char *t)
{
  char resp[257];
  char u[128], p[128];
  char lc[128]={0xFD};
  char pc[128]={0xFE};
  
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

  strncpy(&lc[1],s,127);
  strncpy(&pc[1],t,127);

  adamnet_write(lc,sizeof(lc));  
  adamnet_write(pc,sizeof(pc));
  printf("\n");
}
