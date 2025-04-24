/**
 * @brief Parse JSON from open channel.
 *
 * @return status
 */
#ifdef __ADAM__

#include <eos.h>
#include "network_utils.h"

unsigned char network_parse_json(void)
{
  return eos_write_character_device(NET_DEV,"P",1);
}

#endif