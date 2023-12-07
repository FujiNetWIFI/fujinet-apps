/**
 * @brief base64 FujiNet Functions
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <stdlib.h>
#include <atari.h>
#include "base64.h"
#include "sio.h"

unsigned char base64_encode_input(char* buf, unsigned short len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xD0;
  OS.dcb.dstats   = 0x80;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = len;
  OS.dcb.daux     = len;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_encode_compute(void)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCF;
  OS.dcb.dstats   = 0x00;
  OS.dcb.dbuf     = NULL;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = 0;
  OS.dcb.daux     = 0;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_encode_length(unsigned long *len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCE;
  OS.dcb.dstats   = 0x40;
  OS.dcb.dbuf     = len;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = sizeof(unsigned long);
  OS.dcb.daux     = 0;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_encode_output(char *s, unsigned short len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCD;
  OS.dcb.dstats   = 0x40;
  OS.dcb.dbuf     = s;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = len;
  OS.dcb.daux     = len;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

// ---

unsigned char base64_decode_input(char* buf, unsigned short len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCC;
  OS.dcb.dstats   = 0x80;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = len;
  OS.dcb.daux     = len;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_decode_compute(void)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCB;
  OS.dcb.dstats   = 0x00;
  OS.dcb.dbuf     = NULL;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = 0;
  OS.dcb.daux     = 0;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_decode_length(unsigned long *len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xCA;
  OS.dcb.dstats   = 0x40;
  OS.dcb.dbuf     = len;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = sizeof(unsigned long);
  OS.dcb.daux     = 0;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char base64_decode_output(char *s, unsigned short len)
{  
  OS.dcb.ddevic   = BASE64_DEVICE;
  OS.dcb.dunit    = 1;
  OS.dcb.dcomnd   = 0xC9;
  OS.dcb.dstats   = 0x40;
  OS.dcb.dbuf     = s;
  OS.dcb.dtimlo   = BASE64_TIMEOUT;
  OS.dcb.dbyt     = len;
  OS.dcb.daux     = len;
  siov();
  
  return OS.dcb.dstats; // Return SIO error or success.
}
