/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include "help.h"

const char *helptext=
  "CLS\tClear Screen\n"
  "CD\tChange N: Prefix\n"
  "DEBUG\t1 = ON, 0 = OFF, enable debug output\n"
  "DIR\tShow Directory\n"
  "ERA\tErase file from N:\n"
  "EXIT\tBack to CP/M\n"
  "GET\tRetrieve a file from N:\n"
  "LOGIN\tSet N: Credentials\n"
  "MKDIR\tMake Directory on N:\n"
  "NC\tNetwork Terminal\n"
  "PUT\tStore a file to N:\n"
  "PWD\tPrint Prefix\n"
  ;
  

void help(void)
{
  printf("%s",helptext);
}

