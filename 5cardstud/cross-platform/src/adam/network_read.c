/**
 * Read from channel 
 *
 * @param buf Buffer
 * @param len length
 * @return adamnet status code
 */
#ifdef __ADAM__

#include <eos.h>
#include <string.h>
#include <conio.h>
#include "network_utils.h"

extern unsigned char response[1024];

unsigned short network_read(char *buf, unsigned short len)
{
  unsigned short length = 0;
  DCB *dcb = eos_find_dcb(NET_DEV); // Replace with net device
  unsigned char r=eos_read_character_device(NET_DEV,response,1024);
  if (r == ACK)
  {
    memcpy(buf,response,dcb->len);
    length = dcb->len;
    buf[length] = 0;
  }

  return length;
}

#endif