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
#include <ctype.h>
#include "command.h"
#include "help.h"
#include "cls.h"
#include "pwd.h"
#include "cd.h"
#include "dir.h"
#include "get.h"
#include "put.h"
#include "nc.h"
#include "mkdir.h"
#include "era.h"
#include "login.h"
#include "bye.h"
#include "debug.h"

extern char *pptr;

void uppercase(char *s)
{
  while (*s)
    {
      *s = toupper((unsigned char) *s);
      s++;
    }
}

bool command(char *s, char *t)
{
  uppercase(t);

  if (!strcmp(t,"HELP"))
    help();
  else if (!strcmp(t,"CLS"))
    cls();
  else if (!strcmp(t,"PWD"))
    pwd();
  else if (!strcmp(t,"CD"))
    cd(strtok(NULL," "));
  else if (!strcmp(t,"DIR"))
    dir(strtok(NULL," "));
  else if (!strcmp(t,"EXIT"))
    bye();  
  else if (!strcmp(t,"GET"))
    get(strtok(NULL," "),strtok(NULL," "));
  else if (!strcmp(t,"PUT"))
    put(strtok(NULL," "),strtok(NULL," "));
  else if (!strcmp(t,"NC"))
    nc(strtok(NULL," "));
  else if (!strcmp(t,"MKDIR"))
    mkdir(strtok(NULL," "));
  else if (!strcmp(t,"ERA"))
    era(strtok(NULL," "));
  else if (!strcmp(t,"LOGIN"))
    login(strtok(NULL," "),strtok(NULL," "));
  else if (!strcmp(t,"DEBUG"))
    debug(atoi(strtok(NULL," ")));
  else
    return false;

  return true;
}
