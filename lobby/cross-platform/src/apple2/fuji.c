#ifdef __APPLE2__

#include <conio.h>
#include "sp.h"

unsigned char net;

unsigned char getJsonResponse(char *url, unsigned char* buf, int maxLen) {
 return 0;
}

void initialize() {
  
  unsigned char result = sp_init();
 // if (!result)
 //   cgetc();
  
  //net = sp_find_network();
}
#endif /* __APPLE2__ */