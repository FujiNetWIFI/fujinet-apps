/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>
#include <string.h>
#include "network.h"

unsigned char response[1024];

/**
 * Get Status
 *
 * @param s pointer to Status struct
 * @return adamnet status code
 */

unsigned char network_status(NetStatus *s)
{
    return 0;
}


/**
 * Open Connection
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param open mode
 * @param translation mode (CR/LF to other line endings)
 * @return status
 */



unsigned char network_open(char *url, unsigned char mode, unsigned char translation)
{
    return 0;
}

/**
 * Read from channel
 *
 * @param buf Buffer
 * @param len length
 * @return adamnet status code
 */


unsigned short network_read(char *buf, unsigned short len)
{
    return 0;
}

/**
 * Write to channel
 *
 * @param buf Pointer to buffer, should have +1 more than number of bytes to write.
 * @param len number of actual bytes to write
 * @return number of bytes written
 * @return status
 */


unsigned char network_write(unsigned char *buf, unsigned short len)
{
    return 0;
}

/**
 * Close Connection
 *
 * @return status
 */


unsigned char network_close(void)
{
    return 0;
}
