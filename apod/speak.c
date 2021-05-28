/*
  speak.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module speaks text, using the FujiNet's "P4:"
  'printer' device, which is its built-in S.A.M. speech
  synthesizer.
  (See https://github.com/FujiNetWIFI/fujinet-platformio/wiki/Using-SAM-%28Voice-Synthesizer%29)

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-26 - 2021-05-27
*/

#include <atari.h>
#include <string.h>
#include "sio.h"
#include "speak.h"
#include "screen_helpers.h"

void speak(void)
{
  /* P4: */
  OS.dcb.ddevic = 0x40; /* Serial Bus ID 0x40 = Printer */
  OS.dcb.dunit = 4; /* 4th device */

  OS.dcb.dcomnd = 'W'; /* Write */

  OS.dcb.dstats = 0x80; /* PUT */
  OS.dcb.dbuf = (txt_mem + 40); /* ASCII version of the text */
  OS.dcb.dtimlo = 0x1f;
  OS.dcb.dbyt = 40; /* 40 bytes */
  OS.dcb.daux = 0;
  siov();
}
