/**
 * Perform JSON query
 *
 * @param query The JSONPath string to query
 */
#ifdef __ADAM__

#include <eos.h>
#include <string.h>
#include "network_utils.h"

unsigned char network_json_query(const char *query)
{
  FUJI_JSON_QUERY q;
  char r=ACK;

  memset(response,0,sizeof(response));
  q.cmd = 'Q';
  strncpy(q.query,query,sizeof(q.query));
  r = eos_write_character_device(NET_DEV,q,sizeof(q));

  return r;
}

#endif