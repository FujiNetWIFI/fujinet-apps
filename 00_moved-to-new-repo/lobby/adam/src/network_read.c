/**
 * Read from channel 
 *
 * @param buf Buffer
 * @param len length
 * @return adamnet status code
 */

#include <eos.h>
#include <string.h>
#include <conio.h>
#include "network.h"

extern unsigned char response[1024];

unsigned short network_read(char *buf, unsigned short len)
{
  DCB *dcb = eos_find_dcb(NET_DEV); // Replace with net device
  unsigned char r=eos_read_character_device(NET_DEV,response,1024);
  
  if (r == 0x80)
    {
      memcpy(buf,response,dcb->len);
      return dcb->len;
    }
  else
    return 0;
}
