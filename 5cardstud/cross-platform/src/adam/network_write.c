/**
 * Write to channel 
 *
 * @param buf Pointer to buffer, should have +1 more than number of bytes to write.
 * @param len number of actual bytes to write
 * @return number of bytes written
 * @return status
 */
#ifdef __ADAM__

#include <eos.h>
#include <string.h>
#include <conio.h>
#include "network_utils.h"

unsigned char network_write(unsigned char *buf, unsigned short len)
{
  while (len>0)
  {
    char out[64]={'W'};
    unsigned char l = len > 64 ? 64 : len;
    unsigned char r=0;
    
    memcpy(&out[1],buf,l);

    r=eos_write_character_device(NET_DEV,out,l+1);

    if (r!=0x80)
      break;
    else
      len -= l;
  }
}

#endif