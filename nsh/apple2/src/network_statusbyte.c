#include <stdio.h>
#include "network.h"
#include "sp.h"

/**
 * @brief Get Network Device Status byte 
 * @param buf Buffer to write
 * @param len Buffer length
 * @param returns DCB status
 */

extern unsigned char net;

unsigned char network_statusbyte(void)
{
  sp_status(net,'S');
  printf("%02x\n",sp_payload[2]);
  return sp_payload[2];
}
