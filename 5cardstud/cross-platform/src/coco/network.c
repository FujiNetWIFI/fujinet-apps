#ifdef _CMOC_VERSION_

/*
  Network functionality
*/


#include <cmoc.h>
#include <coco.h>
#include "../platform-specific/appkey.h"
#include "../fujinet-fuji.h"
#include "../fujinet-network.h"
#include "../misc.h"

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

#endif /* _CMOC_VERSION */
