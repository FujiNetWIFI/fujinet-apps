/**
 * @brief Parse JSON from open channel.
 *
 * @return status
 */

#include <eos.h>
#include "network.h"

unsigned char network_parse_json(void)
{
  return eos_write_character_device(NET_DEV,"P",1);
}
