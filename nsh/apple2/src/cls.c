/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#ifndef CLS_H
#define CLS_H

#include <stdio.h>
#include "cls.h"

void cls(void)
{
  printf("\x1b" "E");
}

#endif /* CLS_H */
