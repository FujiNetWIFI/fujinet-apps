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
#include <stdio.h>
#include "../misc.h"

/// @brief Retrieve a json parsed url response
/// @param url 
/// @return response length
uint16_t getJsonResponse(char *url, unsigned char *buffer, uint16_t max_len) {
  static uint16_t count;
  network_open(url, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
  network_json_parse(url);
  count = network_json_query(url, "", buffer);
  network_close(url);
  return count;
}

/// @brief Retrieve the response
/// @param url 
/// @return response length
uint8_t getResponse(char *url, unsigned char *buffer, uint16_t max_len) {
  static int16_t count;
  network_open(url, OPEN_MODE_HTTP_GET_H, OPEN_TRANS_NONE);
  count = network_read(url, buffer, max_len);
  network_close(url);
  return count>0;
}

#endif /* __APPLE2__ */