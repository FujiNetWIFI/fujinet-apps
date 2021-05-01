/*
  interrupt_helpers.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module provides a few helper functions used for
  setting a Vertical Blank Interrupt, and setting and
  enabling, or disabling, a Display List Interrupt.

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-05-01
*/

#include <atari.h>
#include "interrupt_helpers.h"

/* Keep track of old VBI vector, so we can jump to it at
   the end of ours (see below), and restore it when we're done
   needing our VBI */
void * OLDVEC;


#pragma optimize (push, off)

/**
 * Set the deffered VBI vector
 *
 * @param void * Addr the VBI routine's location
 */
void mySETVBV(void * Addr)
{
  OS.critic = 1;
  OS.vvblkd = Addr;
  OS.critic = 0;

  ANTIC.nmien = NMIEN_VBI;
}

/**
 * Activate our Display List Interrupt
 */
void dli_init(void * dli_func)
{
  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli_func;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;
}

/**
 * Deactivate our Display List Interrupt
 */
void dli_clear(void)
{
  ANTIC.nmien = NMIEN_VBI;
}

#pragma optimize (pop)

