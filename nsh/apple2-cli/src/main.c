/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <conio.h>
#include "nsh.h"
#include "sp.h"

unsigned char buf[1024];
unsigned char net;

void banner(void)
{
  clrscr();
  printf("Welcome to N:\n\nTYPE 'HELP' FOR COMMANDS.\n\n");
}

void main(void)
{
  sp_init();
  net = sp_find_network();

  clrscr();
  cursor(1);
  
  if (net == 0)
    {
      printf("COULD NOT FIND NET.");
      cgetc();
      return;
    }
  
  banner();
  
  while (1)
    nsh();
}
