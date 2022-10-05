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
#include "parse.h"
#include "command.h"

char *t = NULL;

extern char cmdline[512];

bool parse(void)
{
  t = strtok(cmdline," ");

  if (t == NULL)
    return false; // empty line
  
  return command(cmdline,t);
}
