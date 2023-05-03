/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include <msx.h>
#include "RetroNET-FileStore.h"

unsigned char response[1024];
int tcpHandle = 0xFF;
bool I_am_host = false;

NetStatus globalStatus;

/*
typedef struct
{
    unsigned short rxBytesWaiting;
    bool connected;
    unsigned char error;
} NetStatus;
*/

/**
 * Get Status
 *
 * @param s pointer to Status struct
 * @return adamnet status code
 */

unsigned char network_init(void)
{
    initNABULib();
    globalStatus.connected = 0;
    globalStatus.error = 0;
    globalStatus.rxBytesWaiting = 0;

    return 0;
}

unsigned char network_status(NetStatus *s)
{
    s->connected = globalStatus.connected;
    s->error = globalStatus.error;
    s->rxBytesWaiting = s->connected ? 0 : 32;

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
    char host[32];
    char url2[32];
    int port;
    int file_handle = 0xFF;
    char *data;

    mode = mode;
    translation = translation;

    strcpy(url2, url);
    data = strrchr(url2, ':');
    if (data != NULL)
    {
        sscanf(data+1, "%d", & port);
        *data = '\0';
        sscanf(url2, "N:TCP://%s", host);
        if (strcmp(host, "") != 0)
        {
            tcpHandle = rn_TCPOpen(strlen(host), host, port, file_handle);
            if (tcpHandle == 0xFF)
            {
                globalStatus.connected = 0;
            } else
            {
                globalStatus.connected = 1;
            }
            I_am_host = false;
        } else
        {
            tcpHandle = 0xFE;
            globalStatus.connected = 1;
            I_am_host = true;
        }
    }
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
    int chars_read = -1, index = 0;
    long timeout = 2000;
    unsigned short err;

    memset(buf, '\0', len);

    do
    {
        if (I_am_host)
        {
            chars_read = rn_TCPServerRead(&buf[index], 0, len);
            if (chars_read == 255)
                chars_read = -1;

            if (chars_read == 0)
                chars_read = -1;
        } else
        {
            chars_read = rn_TCPHandleRead(tcpHandle, &buf[index], 0, len);

            if (chars_read == 255)
                chars_read = -1;
            if (chars_read == 0)
                chars_read = -1;
        }

        if (index == len)
            break;

        if (chars_read == -1)
        {
            timeout--;
        } else
        {
            index += chars_read;
        }

    } while ((chars_read == -1) && (timeout > 0));

    if (timeout <= 0)
        err = NET_ERR_RECV_TMOUT;
    else
        err = NET_ERR_OK;

    globalStatus.error = err;

    return err;
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
    if (I_am_host)
        rn_TCPServerWrite(0, len, buf);
    else
        rn_TCPHandleWrite(tcpHandle, 0, len, buf);
    return 0;
}

/**
 * Close Connection
 *
 * @return status
 */


unsigned char network_close(void)
{
    if (! I_am_host)
        rn_TCPHandleClose(tcpHandle);
    tcpHandle = 0xFF;
    globalStatus.connected = 0;
    globalStatus.error = NET_ERR_OK;

    return 0;
}
