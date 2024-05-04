#ifdef __APPLE2__

/*
  Network functionality
*/
#include <apple2.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "../platform-specific/appkey.h"
#include "../fujinet-fuji.h"
#include "../fujinet-network.h"


/// @brief Retrieve a json parsed url response
/// @param url 
/// @return response length
int getJsonResponse(char *url, unsigned char *buffer, int max_len) {
  int err,count = 0;
  bzero(buffer, max_len);
  err = network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  err = network_json_parse(url);
  count = network_json_query(url, "", buffer);
  err = network_close(url);
  return count;
}

#endif /* __APPLE2__ */