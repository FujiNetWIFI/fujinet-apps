#ifdef _CMOC_VERSION_

/*
  Network functionality
*/
//#include <apple2.h>
//#include <string.h>
//#include <conio.h>
//#include <stdbool.h>
#include "../platform-specific/appkey.h"
#include "../fujinet-fuji.h"
#include "../fujinet-network.h"
//#include <stdio.h>
#include "../misc.h"

/// @brief Retrieve a json parsed url response
/// @param url 
/// @return response length
uint16_t getJsonResponse(char *url, unsigned char *buffer, uint16_t max_len) {
  static uint16_t count;
  // TODO FIXME  network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  // TODO FIXME  network_json_parse(url);
  // TODO FIXME  count = network_json_query(url, "", buffer);
  // TODO FIXME network_close(url);
  return count;
}

#endif

