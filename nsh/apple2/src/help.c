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
  "40         40 Column Mode\n"
  "80         80 Column Mode\n"
  "CLS        Clear Screen\n"
  "CD         Change N: Prefix\n"
  "DIR        Show Directory\n"
  "ERA        Erase file from N:\n"
  "EXIT       Back to PRODOS\n"
  "GET        Retrieve a file from N:\n"
  "LOGIN      Set N: Credentials\n"
  "MKDIR      Make Directory on N:\n"
  "NC         Network Terminal\n"
  "PUT        Store a file to N:\n"
  "PWD        Print Prefix\n"
  ;
  

void help(void)
{
  printf("%s",helptext);
}

