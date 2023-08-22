/**
 * Perform JSON query
 *
 * @param query The JSONPath string to query
 */

#include <eos.h>
#include <string.h>
#include "network.h"

unsigned char network_json_query(const char *query)
{
  char q[256];
  char r=ACK;

  memset(response,0,sizeof(response));
  memset(q,0,sizeof(q));
  q[0] = 'Q';
  strncat(q,query,sizeof(q));

  eos_write_character_device(NET_DEV,q,sizeof(q));

  return r;
}
