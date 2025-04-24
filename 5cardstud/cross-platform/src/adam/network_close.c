/**
 * Close Connection
 *
 * @return status
 */
#ifdef __ADAM__

#include <eos.h>
#include <string.h>
#include "network_utils.h"

unsigned char network_close(void)
{
  char resp='C';
  
  return eos_write_character_device(NET_DEV,&resp,1);
}

#endif