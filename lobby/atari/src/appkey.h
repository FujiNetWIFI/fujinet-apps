/**
 * @brief   App Key Functions
 * @author  Oscar Fowler, Thomas Cherryhomes, Eric Carr
 * @license gpl v. 3
 */

#ifndef APPKEY_H
#define APPKEY_H

#include <atari.h>
#include "sio.h"

#define SIO_DEVICEID 0x70
#define SIO_UNITID 0x01
#define SIO_TIMEOUT 0x09 // (64/60)-second unit values

#define SIO_XFER_NONE 0x00
#define SIO_XFER_READ 0x40
#define SIO_XFER_WRITE 0x80

#define SIO_CMD_APPKEY_WRITE 0xDE
#define SIO_CMD_APPKEY_READ 0xDD
#define SIO_CMD_APPKEY_OPEN 0xDC
#define SIO_CMD_APPKEY_CLOSE 0xDB

#define MAX_APPKEY_LEN 64

#define APPKEY_READ  0
#define APPKEY_WRITE 1

/**
 * @brief Convenience type for handling an appkey
 */
typedef union {
    struct
    {
        unsigned int creator;
        unsigned char app;
        unsigned char key;
        unsigned char mode;
        unsigned char reserved;
    } open;
    struct
    {
        unsigned int length;
        unsigned char value[MAX_APPKEY_LEN];
        
    } read;
    struct
    {
        unsigned char value[MAX_APPKEY_LEN];
    } write;
} appkeyblock;

/**
 * @brief Read a key into appkey var appkey
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @brief Target pointer for appkey data
 */
unsigned char sio_readkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data);

/**
 * @brief write a key from appkey var appkey
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @param Data to write into key
 */
unsigned char sio_writekey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data);

#endif /* APPKEY_H */
