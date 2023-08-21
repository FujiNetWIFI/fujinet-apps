/**
 * @brief Appkey Routines
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef APPKEY_H
#define APPKEY_H

#define ADAMNET_SEND_APPKEY_READ  0xDD
#define ADAMNET_SEND_APPKEY_WRITE 0xDE

#define MAX_APPKEY_LEN 64

#define ADAMNET_FUJI_DEVICE_ID 0x0F

unsigned char appkey_read(unsigned int creator, unsigned char app, unsigned char key, char *buf);
unsigned char appkey_write(unsigned int creator, unsigned char app, unsigned char key, char *buf);

#endif /* APPKEY_H */
