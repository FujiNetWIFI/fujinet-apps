/**
 * Read from channel 
 *
 * @param buf Buffer
 * @param len length
 * @return smartport error code (e.g. SP_ERR_NOERROR)
 */

#include <string.h>
#include "network.h"
#include "sp.h"

extern unsigned char net;

#define MAX_READ_SIZE 512

unsigned char network_read(unsigned char *b, unsigned short len)
{
  unsigned char err = SP_ERR_NOERROR;
  unsigned short o = 0;
  
  while (len > 0)
    {
      unsigned short l=MAX_READ_SIZE;

      if (len < MAX_READ_SIZE)
	l=len;
      
      err = sp_read(net,l); // Data is in sp_payload[]
      
      if (err != SP_ERR_NOERROR)
	return err;

      memcpy(b,&sp_payload[o],l);
      o += l;
      len -= l;
    }
  
  return SP_ERR_NOERROR;
}
