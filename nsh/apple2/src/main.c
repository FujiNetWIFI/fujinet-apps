/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include "nsh.h"

void banner(void)
{
  printf("\x1b" "E" "Welcome to N:\n\nTYPE 'HELP' FOR COMMANDS.\n\n");
}

void main(void)
{
  banner();
  
  while (1)
    nsh();
}
