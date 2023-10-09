/**
 * Write to network
 *
 * @param buf Buffer
 * @param len length
 * @return smartport error code (e.g. SP_ERR_OK)
 */

#include <string.h>
#include "network.h"
#include "sp.h"

extern unsigned char net;

#define MAX_WRITE_SIZE 512

unsigned char network_write(unsigned char *b, unsigned short len)
{
  unsigned char err = SP_ERR_OK;

  while (len > 0)
  {
    unsigned short l = MAX_WRITE_SIZE;

    if (len < MAX_WRITE_SIZE)
      l = len;

    memcpy(&sp_payload[0], b, l);
    b += l;

    err = sp_write(net, l); // Data is now in sp_payload[]

    if (err != SP_ERR_OK)
      return err;
  }

  return SP_ERR_OK;
}
