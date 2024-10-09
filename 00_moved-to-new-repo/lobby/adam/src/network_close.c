/**
 * Close Connection
 *
 * @return status
 */

#include <eos.h>
#include <string.h>
#include "network.h"

unsigned char network_close(void)
{
  char resp='C';
  
  return eos_write_character_device(NET_DEV,&resp,1);
}
