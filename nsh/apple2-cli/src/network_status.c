#include <stdio.h>
#include "network.h"
#include "sp.h"

/**
 * @brief  Get Network Device Status byte 
 * @param  ptr to where to put bytes waiting
 * @param  ptr to where to put connection status
 * @param  ptr to where to put network error byte.
 * @return smartport status (e.g. SP_ERR_OK)
 */

extern unsigned char net;

unsigned char network_status(unsigned short *bw, unsigned char *c, unsigned char *nerr)
{
  unsigned char err = sp_status(net,'S');

  if (bw != NULL)
    *bw = (sp_payload[0]) | (sp_payload[1] << 8);

  if (c != NULL)
    *c = sp_payload[2];

  if (nerr != NULL)
    *nerr = sp_payload[3];
  
  return err;
}
