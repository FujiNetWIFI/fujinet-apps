/**
 * Open Connection
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param open mode
 * @param translation mode (CR/LF to other line endings)
 * @return status
 */

#include <stdio.h>
#include <string.h>
#include "network.h"
#include "sp.h"
#include "debug.h"

extern unsigned char net;

unsigned char network_open(char *url, unsigned char mode, unsigned char translation)
{
  char *p = (char *)&sp_payload[3];
  memset(sp_payload,0,sizeof(sp_payload));

  sp_open(net);
  sp_payload[0]=0x02; // 258 bytes
  sp_payload[1]=0x01;
  sp_payload[2]=mode; // READ/WRITE (or GET)
  sp_payload[3]=translation; // NO TRANSLATION
  memcpy(&sp_payload[4],url,256);

  return sp_control(net,'O');
}
