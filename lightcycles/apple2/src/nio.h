/**
 * @brief   Network I/O Functions for #FujiNet
 * @verbose Using the SmartPort
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 *
 * @license gpl v. 3
 */

#ifndef NIO_H
#define NIO_H

#include <stdbool.h>

#define CHANNEL_MODE_NORMAL 0
#define CHANNEL_MODE_JSON 1

/**
 * @brief Open  Network connection
 * @param net   Network device ID
 * @param url   The URL to open (e.g. N:TCP://1.2.3.4:6502/)
 * @param mode  The opening mode (e.g. 12 = read/write)
 * @param trans The translation mode (usually 0)
 */
char nopen(unsigned char net, char *url, unsigned char mode, unsigned char trans);

/**
 * @brief  Close Network Connection
 * @return SmartPort error code, or 0 if none.
 */
char nclose(unsigned char net);

/**
 * @brief  Get Network Status
 * @param  net The smartport network device ID to request status
 * @return # of bytes waiting, connected?, and last known error in ptrs, 
 *         smartport error in return or 0 if ok
 */
char nstatus(unsigned char net, unsigned short *bw, bool *connected, unsigned char *err);

/**
 * @brief  Read requested # of bytes into buf
 * @param  net The smartport Network device ID 
 * @param  buf The target buffer, assumed to be large enough
 * @param  len The # of bytes to read into buffer, assumed < buf len
 * @return smartport error, or 0 if ok.
 */
char nread(unsigned char net, void *buf, unsigned short len);

/**
 * @brief  send requested # of bytes from buf
 * @param  net The smartport Network device ID 
 * @param  buf The source buffer, assumed to be large enough
 * @param  len The # of bytes to send from buffer, assumed < buf len
 * @return smartport error, or 0 if ok.
 */
char nwrite(unsigned char net, void *buf, unsigned short len);

/**
 * @brief  Send control command to network device
 * @param  net the smartport network device ID
 * @param  cmd The command to do.
 * @param  buf The command buffer to send
 * @param  len The length of command buffer
 * @return smartport error, or 0 if ok.
 */
char ncontrol(unsigned char net, unsigned char cmd, void *buf, unsigned short len);

/**
 * @brief Send open channel to JSON parser
 * @param net  The network device
 * @return smartport error, or 0 if ok.
 */
char njsonparse(unsigned char net);

/**
 * @brief JSON-Pointer for JSON path
 * @param net  The Network smartport device ID
 * @param path The JSON-Pointer path a la RFC 6901
 */
char njsonquery(unsigned char net, char *path);

#endif /* NIO_H */
