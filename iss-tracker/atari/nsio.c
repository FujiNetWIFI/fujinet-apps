/*
  This module provides "nXXXX()" functions for
  talking to the #FujiNet N: device via direct SIO Calls.

  From "Network Testing tools"
  by Thomas Cherryhomes <thom.cherryhomes@gmail.com>
  (Released under GPL 3.0)

  Last modified: 2022-05-27
*/

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "sio.h"
#include "nsio.h"

#define FUJINET_SIO_DEVICEID 0x71

#define TIMEOUT 0x1f

#define DSTATS_XFER_NONE 0x00 /* No payload */
#define DSTATS_XFER_RECV 0x40 /* Atari receives (#FujiNet to Atari) */
#define DSTATS_XFER_SEND 0x80 /* Atari sends (Atari to #FujiNet) */

#define AUX2_NO_TRANSLATION 0x00

unsigned char nopen(unsigned char unit, char* buf, unsigned char aux1)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'O';
  OS.dcb.dstats = DSTATS_XFER_SEND;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = aux1;
  OS.dcb.daux2 = AUX2_NO_TRANSLATION;
  return siov();
}

unsigned char nclose(unsigned char unit)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'C';
  OS.dcb.dstats = DSTATS_XFER_NONE;
  OS.dcb.dbuf = NULL;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = 0;
  OS.dcb.daux1 = 0;
  OS.dcb.daux2 = 0;
  return siov();
}

unsigned char nread(unsigned char unit, char* buf, unsigned short len)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'R';
  OS.dcb.dstats = DSTATS_XFER_RECV;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = len;
  OS.dcb.daux = len;
  return siov();
}

unsigned char nwrite(unsigned char unit, char* buf, unsigned short len)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'W';
  OS.dcb.dstats = DSTATS_XFER_SEND;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = len;
  OS.dcb.daux = len;
  return siov();
}

unsigned char nstatus(unsigned char unit)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'S';
  OS.dcb.dstats = DSTATS_XFER_RECV;
  OS.dcb.dbuf = OS.dvstat;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = sizeof(OS.dvstat);
  OS.dcb.daux1 = 0;
  OS.dcb.daux2 = 0;
  return siov();
}

unsigned char nchanmode(unsigned char unit, unsigned char aux1, unsigned char mode)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 0xFC;
  OS.dcb.dstats = DSTATS_XFER_NONE;
  OS.dcb.dbuf = NULL;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = 0;
  OS.dcb.daux1 = aux1;
  OS.dcb.daux2 = mode;
  return siov();
}

unsigned char njsonparse(unsigned char unit, unsigned char aux1)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'P';
  OS.dcb.dstats = DSTATS_XFER_NONE;
  OS.dcb.dbuf = NULL;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = 0;
  OS.dcb.daux1 = aux1;
  OS.dcb.daux2 = AUX2_NO_TRANSLATION;
  return siov();
}

unsigned char njsonquery(unsigned char unit, unsigned char aux1, char *buf)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'Q';
  OS.dcb.dstats = DSTATS_XFER_SEND;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = TIMEOUT;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = aux1;
  OS.dcb.daux2 = AUX2_NO_TRANSLATION;
  return siov();
}
