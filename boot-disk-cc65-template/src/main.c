/**
 * main for cc65 boot disk template
 *
 * Author:
 *  Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL 3.0, read LICENSE in top
 * level folder for details.
 */

#include <atari.h>
#include <stdlib.h>
#include "conio.h"

void main(void)
{
  print("Booted! ");
  while (1)
    {
      OS.color2++;
    }
}
