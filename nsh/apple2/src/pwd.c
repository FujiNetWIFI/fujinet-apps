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
#include "pwd.h"
#include "sp.h"

extern unsigned char net;

void pwd(void)
{
  sp_status(net,'0');
  printf("%s\n",sp_payload);
}
