/**
 * Open Connection
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param open mode
 * @param translation mode (CR/LF to other line endings)
 * @return status
 */
#ifdef __ADAM__

#include <eos.h>
#include <string.h>
#include "network_utils.h"

#define READ_WRITE 12

unsigned char network_open(char *url, unsigned char mode, unsigned char translation)
{
  FUJI_CMD co;
  memset(co,0,sizeof(co));
  co.cmd = 'O';
  co.mode = READ_WRITE;
  co.trans = translation;
  strcpy(co.url,url);
  return eos_write_character_device(NET_DEV,co,sizeof(co));
}

#endif
