/**
 * N: I/O
 */

#include "nio.h"
#include "sio.h"
#include <atari.h>
#include <stddef.h>

unsigned char nunit(char* devicespec)
{
  unsigned char unit=1;
  
  // Set unit to 1 unless explicitly specified.
  if (devicespec[1]==':')
    unit=1;
  else if (devicespec[2]==':')
    unit=devicespec[1]-0x30; // convert from alpha to integer.
  else
    unit=1;

  return unit;
}

unsigned char nopen(char* devicespec, unsigned char trans)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='O';
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=devicespec;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=12;
  OS.dcb.daux2=trans;
  siov();

  if (OS.dcb.dstats!=1)
    {
      if (OS.dcb.dstats==144)
	{
	  nstatus(devicespec);
	  return OS.dvstat[3]; // return extended error.
	}
      else
	return OS.dcb.dstats; // Return SIO error.
    }
}

unsigned char nclose(char* devicespec)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='C';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      if (OS.dcb.dstats==144)
	{
	  nstatus(devicespec);
	  return OS.dvstat[3]; // return extended error.
	}
      else
	return OS.dcb.dstats; // Return SIO error.
    }
}

unsigned char nstatus(char* devicespec)
{
  unsigned char unit=nunit(devicespec);

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='S';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=OS.dvstat;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=sizeof(OS.dvstat);
  OS.dcb.daux=0;
  siov();

  return OS.dcb.dstats == 1 ? 1 : OS.dvstat[3];
}

unsigned char nread(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='R';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=OS.dcb.daux=len;
  siov();

  if (OS.dcb.dstats!=1)
    {
      if (OS.dcb.dstats==144)
	{
	  nstatus(devicespec);
	  return OS.dvstat[3]; // return extended error.
	}
      else
	return OS.dcb.dstats; // Return SIO error.
    }
}

unsigned char nwrite(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='W';
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=OS.dcb.daux=len;
  siov();

  if (OS.dcb.dstats!=1)
    {
      if (OS.dcb.dstats==144)
	{
	  nstatus(devicespec);
	  return OS.dvstat[3]; // return extended error.
	}
      else
	return OS.dcb.dstats; // Return SIO error.
    }
}
