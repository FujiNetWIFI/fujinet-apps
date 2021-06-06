/*
  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module provides "nXXXX()" functions for
  talking to the #FujiNet N: device via direct SIO Calls.

  From "Network Testing tools"
  by Thomas Cherryhomes <thom.cherryhomes@gmail.com>
  (Released under GPL 3.0)

  Last modified: 2021-06-05
*/

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "sio.h"
#include "nsio.h"

#define FUJINET_SIO_DEVICEID 0x71

unsigned char nopen(unsigned char unit, char* buf, unsigned char aux1)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd = 'O';
  OS.dcb.dstats = 0x80;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = 0x1f;
  OS.dcb.dbyt = 256;
  OS.dcb.daux1 = aux1;
  OS.dcb.daux2 = 0; // NO TRANSLATION!
  return siov();
}

unsigned char nclose(unsigned char unit)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'C';
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = NULL;
  OS.dcb.dtimlo = 0x1f;
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
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = 0x1f;
  OS.dcb.dbyt = len;
  OS.dcb.daux = len;
  return siov();
}

unsigned char nwrite(unsigned char unit, char* buf, unsigned short len)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'W';
  OS.dcb.dstats = 0x80;
  OS.dcb.dbuf = buf;
  OS.dcb.dtimlo = 0x1f;
  OS.dcb.dbyt = len;
  OS.dcb.daux = len;
  return siov();
}

unsigned char nstatus(unsigned char unit)
{
  OS.dcb.ddevic = FUJINET_SIO_DEVICEID;
  OS.dcb.dunit = unit;
  OS.dcb.dcomnd = 'S';
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = OS.dvstat;
  OS.dcb.dtimlo = 0xff;
  OS.dcb.dbyt = 4;
  OS.dcb.daux1 = 0;
  OS.dcb.daux2 = 0;
  return siov();
}

