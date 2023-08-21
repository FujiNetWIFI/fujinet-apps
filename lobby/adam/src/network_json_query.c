/**
 * Perform JSON query
 *
 * @param query The JSONPath string to query
 * @param buf the output buffer
 * @return len the output buffer length
 */

#include <eos.h>
#include <string.h>
#include "network.h"

unsigned char network_json_query(const char *query, char *buf, unsigned short len)
{
  char q[256];

  memset(q,0,sizeof(q));
  q[0] = 'Q';
  strncat(q,buf,sizeof(q));

  return eos_write_character_device(NET_DEV,q,sizeof(q));
}
