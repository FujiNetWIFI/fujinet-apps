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

// A temp version that does not add 256 bytes to the stack
uint8_t network_open_temp(const char* devicespec, uint8_t mode, uint8_t trans)
{
    struct _o
    {
        uint8_t opcode;
        uint8_t unit;
        uint8_t cmd;
        uint8_t mode;
        uint8_t trans;
    } o;

    o.opcode =  0xE3;
    o.unit = network_unit(devicespec);
    o.cmd = 'O';
    o.mode = mode;
    o.trans = trans;

    bus_ready();
    dwwrite((uint8_t *)&o, sizeof(o));
    dwwrite((uint8_t *)devicespec, 256);
    
    return 0;//network_get_error(o.unit);
}

/// @brief Retrieve the response
/// @param url
/// @return response length
uint8_t getResponse(char *url, unsigned char *buffer, uint16_t max_len) {
    static int16_t count;
   

    network_open_temp(url, OPEN_MODE_HTTP_GET_H, OPEN_TRANS_NONE);
    count = network_read(url, buffer, max_len);
    network_close(url);
    return count>0;
}

#endif /* _CMOC_VERSION */
