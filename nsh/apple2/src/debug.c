/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdio.h>
#include "debug.h"

bool _debug = false;

void debug(bool b)
{
  printf("\nDEBUG %s\n",b ? "ON" : "OFF");
  _debug = b;
}

bool debug_enabled(void)
{
  return _debug;
}
