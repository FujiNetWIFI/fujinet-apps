/**
 * N: I/O
 */

#include "nio.h"
#include "sio.h"
#include <atari.h>
#include <stddef.h>

#define TIMEOUT 0x1f /* approx 30 seconds */

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

  OS.dcb.ddevic   = DFUJI;      // Fuji Device Identifier
  OS.dcb.dunit    = unit;       // Unit number integer 1 through 4
  OS.dcb.dcomnd   = 'O';        // Open
  OS.dcb.dstats   = DWRITE;     // sending to to SIO device
  OS.dcb.dbuf     = devicespec; // eg: N:TCP//
  OS.dcb.dtimlo   = TIMEOUT;    // approximately 30 second timeout
  OS.dcb.dbyt     = 256;        // max size of our device spec
  OS.dcb.daux1    = OUPDATE;    // Read and write
  OS.dcb.daux2    = trans;      // CR/LF translation
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success
}

unsigned char nclose(char* devicespec)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'C';      // Close
  OS.dcb.dstats   = 0x00;
  OS.dcb.dbuf     = NULL;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = 0;
  OS.dcb.daux     = 0;
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char nstatus(char* devicespec)
{
  unsigned char unit=nunit(devicespec);

  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'S';    // status
  OS.dcb.dstats   = DREAD;
  OS.dcb.dbuf     = OS.dvstat;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = sizeof(OS.dvstat);
  OS.dcb.daux     = 0;
  siov();

  return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended status
}

unsigned char nread(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'R';    // read
  OS.dcb.dstats   = DREAD;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = OS.dcb.daux = len; // Set the buffer size AND daux with length
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char nwrite(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);
  
  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'W';    // write
  OS.dcb.dstats   = DWRITE;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = OS.dcb.daux = len;
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success.
}
