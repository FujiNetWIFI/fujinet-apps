/**
 * Open Connection
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param open mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
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
  unsigned char i;
  memset(sp_payload,0,sizeof(sp_payload));

  sp_open(net);
  sp_payload[0]=strlen(url)+2; // url + mode & translation
  sp_payload[1]=0x00;
  sp_payload[2]=mode;
  sp_payload[3]=translation;
  memcpy(&sp_payload[4],url,strlen(url));
  sp_payload[4+strlen(url)]=0x9B;
  
  return sp_control(net,'O');
}
