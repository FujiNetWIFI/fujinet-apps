/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "bye.h"

void bye(void)
{
  printf("Bye!\n");
  printf("Press any key to return to PRODOS.");
  cgetc();
  exit(0);
}
