#ifdef __ADAM__
#include <eos.h>
#include "network_utils.h"

/*
  Network functionality
*/

extern unsigned char response[];

int getJsonResponse(char *url, char *buffer, int max_length)
{
  NetStatus stat;
  unsigned char r = 0;
  int rxcount = 0;
  memset(response,0, 1024);

  if (network_open(url,MODE_READ_WRITE,2) != ACK)
    return 0;

  network_set_channel_mode(MODE_JSON);
  network_parse_json();

  //we need to skip the server address part of the url (I think), 
  //so since we already have that in serverEndpoint to find the length and bump past here.
  // (otherwise we'd have to count '/' or some other thing, which might be different)
#ifdef DEBUG_ADAMNET
  printf("query it %s",url);
#endif
  network_json_query(url);

#ifdef DEBUG_ADAMNET
  printf("check status");
#endif //DEBUG_ADAMNET

  rxcount =  network_read(buffer,max_length);
  return rxcount;
}

#endif /* __ADAM__ */
