/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>
#include <eos.h>
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
    char c = 'S';
    unsigned char r = 0;

    r = eos_write_character_device(NET_DEV, &c, 1);

    if (r != 0x80) // If not success, return write error.
        return r;

    r = eos_read_character_device(NET_DEV, response, sizeof(response));

    memcpy(s, response, sizeof(NetStatus));

    return r;
}

-- -
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
    char resp[259];

    resp[0] = 'O';
    resp[1] = mode;
    resp[2] = translation;

    strncpy(&resp[3], url, 256);

    return eos_write_character_device(NET_DEV, resp, 259);
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
    DCB *dcb = eos_find_dcb(0x09); // Replace with net device
    unsigned char r = eos_read_character_device(0x09, response, sizeof(response));

    if (r == 0x80)
    {
        memcpy(buf, response, dcb->len);
        return dcb->len;
    }
    else
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
    while (len > 0)
    {
        char out[64] = {'W'};
        unsigned char l = len > 64 ? 64 : len;
        unsigned char r = 0;

        memcpy(&out[1], buf, l);

        r = eos_write_character_device(NET_DEV, out, l + 1);

        if (r != 0x80)
            break;
        else
            len -= l;
    }
}

/**
 * Close Connection
 *
 * @return status
 */


unsigned char network_close(void)
{
    char resp = 'C';

    return eos_write_character_device(NET_DEV, &resp, 1);
}
