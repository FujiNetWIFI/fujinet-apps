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
#include "adamnet_write.h"
#include "debug.h"

extern unsigned char response[1024];

unsigned char network_open(char *url, unsigned char mode, unsigned char translation)
{
  char *p = &response[3];
  memset(response,0,sizeof(response));
  
  response[0]='O';
  response[1]=mode;
  response[2]=translation;
  
  strncpy(p,url,1024);

  if (debug_enabled())
    printf("\nNETWORK OPEN MODE %u TRANS %u URL %s\n",mode,translation,url);

  return adamnet_write(response,strlen(p)+3);
}
