/**
 * Close Connection
 *
 * @return status
 */

#include <stdio.h>
#include "network.h"
#include "debug.h"
#include "sp.h"

extern unsigned char net;

unsigned char network_close(void)
{  
  return sp_control(net,'C');
}
