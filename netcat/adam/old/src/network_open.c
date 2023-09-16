/**
 * Open Connection
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param open mode
 * @param translation mode (CR/LF to other line endings)
 * @return status
 */

#include <eos.h>
#include <string.h>
#include "network.h"

unsigned char network_open(char *url, unsigned char mode, unsigned char translation)
{
  char resp[259];
  
  resp[0]='O';
  resp[1]=mode;
  resp[2]=translation;

  strncpy(&resp[3],url,256);

  return eos_write_character_device(NET_DEV,resp,259);
}
