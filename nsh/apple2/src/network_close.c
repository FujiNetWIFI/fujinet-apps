/**
 * Close Connection
 *
 * @return status
 */

#include <stdio.h>
#include "adamnet_write.h"
#include "network.h"
#include "debug.h"

unsigned char network_close(void)
{
  unsigned char resp='C';

  if (debug_enabled())
    printf("\nNETWORK CLOSE\n");
  
  return adamnet_write(&resp,1);
}
