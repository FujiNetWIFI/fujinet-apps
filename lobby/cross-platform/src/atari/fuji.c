#ifdef __ATARI__

/**
 * Fuji Device ($70) Commands
 */

#include <stdint.h>
#include <stdbool.h>
#include <atari.h>
#include <string.h>

#include "nio.h"
#include "../fuji.h"


char nUrl[255];

unsigned char getJsonResponse(char *url, unsigned char* buf, int maxLen) {
  static int dataLen=0;
  static bool hasError;
  
  strcpy(nUrl, "N:");
  strcat(nUrl, url);

  hasError = (njsonparse(nUrl, 2)) != SUCCESS ||
    (njsonquery(nUrl, "N:\x9b")) != SUCCESS ||
    nstatus(nUrl) > 128 ||
    (dataLen = (OS.dvstat[1] << 8) + OS.dvstat[0]) == 0;
  
  if (!hasError) {
    if (dataLen+1>maxLen)
      dataLen=maxLen-1;
   
    if (nread(nUrl, buf, dataLen) != 1) {
      hasError = true;
    } 

    nclose(nUrl);
  }
  
  return !hasError ? 0 : OS.dvstat[3];
}


void initialize() {
  unsigned char i;
  OS.soundr=0; // Silent noisy SIO
  for(i=0;i<60;i++)
    waitvsync();
}

#endif /* __ATARI__ */