/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "nsh.h"
#include "input.h"
#include "parse.h"

const char prompt='*';

char cmdline[512];

void nsh(void)
{
  memset(cmdline,0,sizeof(cmdline));
  
  putchar(prompt);

  input(cmdline);

  if (parse() == false)
    printf("%s?\n\n",cmdline);
}
