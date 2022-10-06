/**
 * Write to channel 
 *
 * @param buf Pointer to buffer, should have +1 more than number of bytes to write.
 * @param len number of actual bytes to write
 * @return number of bytes written
 * @return status
 */

#include <string.h>
#include <conio.h>
#include "network.h"
#include "sp.h"

extern unsigned char net;

unsigned char network_write(char *buf, unsigned short len)
{
  unsigned short offset=2;
  
  while (len>0)
    {
      unsigned short i=len;
      
      memset(sp_payload,0,sizeof(sp_payload));

      if (i>sizeof(sp_payload))
	i=sizeof(sp_payload)-2;

      memcpy(&sp_payload[offset],buf,i);

      sp_payload[0]=i & 0xFF;
      sp_payload[1]=i >> 8;

      sp_control(net,'W');
      
      offset += i+2;
      len -= i;
    }

  return offset;
}

