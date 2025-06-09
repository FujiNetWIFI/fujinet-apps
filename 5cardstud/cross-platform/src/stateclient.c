#ifdef _CMOC_VERSION_
#include <cmoc.h>
#define true 1
#define false 0
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif /* CMOC_VERSION */
#define POKE(addr,val)     (*(unsigned char*) (addr) = ((unsigned char)(val)))
#define POKEW(addr,val)    (*(unsigned*) (addr) = ((unsigned)(val)))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))
#include "stateclient.h"
#include "misc.h"
#include "platform-specific/network.h"
#include <conio.h>

char urlBuffer[128];

unsigned char apiCall(const char *path) {
#ifndef __CBM__
  if (serverEndpoint[0] != 'N') {
    strcpy(urlBuffer,"N:");
  } else {
    strcpy(urlBuffer,"");
  }
#else
  memset(urlBuffer,0,sizeof(urlBuffer));
#endif /* ! __CBM__ */
  
  strcat(urlBuffer, serverEndpoint);
  strcat(urlBuffer, path);
  strcat(urlBuffer, query);

  // Set Binary mode
  strcat(urlBuffer, query[0] ? "&bin=1" QUERY_SUFFIX : "?bin=1" QUERY_SUFFIX);
  //printf(urlBuffer);
  //printf("\r\n");
  return getResponse(urlBuffer, &clientState.firstByte, sizeof(clientState.game));
  
}

unsigned char getStateFromServer()
{
  if (requestedMove) {
    strcpy(tempBuffer, "move/");
    strcat(tempBuffer, requestedMove);
    requestedMove=NULL;
  } else {
    strcpy(tempBuffer, "state");
  }

  return apiCall(tempBuffer);
}
