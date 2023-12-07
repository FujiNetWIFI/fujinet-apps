/**
 * @brief Set channel mode
 *
 * @param mode The channel mode (0=PROTOCOL, 1=JSON)
 * @return status
 */

#include <eos.h>
#include "network.h"

struct _scm
{
  unsigned char cmd;
  unsigned char mode;
} SCM; // set channel mode

unsigned char network_set_channel_mode(unsigned char mode)
{
  SCM.cmd  = 0xFC; // Set channel mode
  SCM.mode = mode; // to JSON mode

  return eos_write_character_device(NET_DEV,(unsigned char *)SCM,sizeof(SCM));
}
