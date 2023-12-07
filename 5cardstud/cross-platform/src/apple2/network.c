#ifdef __APPLE2__

/*
  Network functionality
*/
#include <apple2.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "../platform-specific/appkey.h"

/// @brief Retrieve a json parsed url response
/// @param url 
/// @return response length
int getJsonResponse(char *url, unsigned char *buffer, int max_len) {
  static int i, len;
  return len;
}

#endif /* __APPLE2__ */