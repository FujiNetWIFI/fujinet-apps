/**
 * Get Status
 *
 * @param s pointer to Status struct
 * @return adamnet status code
 */

#include <eos.h>
#include <string.h>
#include "network.h"

extern unsigned char response[1024];

unsigned char network_status(NetStatus *s)
{
  char c='S';
  unsigned char r=0;
  
  r=eos_write_character_device(NET_DEV,&c,1);

  if (r!=0x80) // If not success, return write error.
    return r;

  r=eos_read_character_device(NET_DEV,response,sizeof(response));

  memcpy(s,response,sizeof(NetStatus));

  return r;
}
