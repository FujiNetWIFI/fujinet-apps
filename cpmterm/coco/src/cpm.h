/**
 * @brief   CPM
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose CPM Interface
 */

#ifndef CPM_H
#define CPM_H

/**
 * @brief wait for network device ready
 */
void cpm_ready(void);

/**
 * @brief Get response data from last command
 * @param buf Target buffer 
 * @param len Length 
 */
byte cpm_get_response(byte *buf, int len);

/**
 * @brief Ask CP/M device to boot CP/M 
 */
void cpm_boot(void);

/**
 * @brief Return CP/M status (# of bytes waiting in RX queue)
 * @return # of bytes waiting in RX queue.
 */
unsigned short cpm_status(void);

/**
 * @brief read from CP/M
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte cpm_read(byte *buf, unsigned int len);

/**
 * @brief write to CP/M
 * @param buf Destination buffer (must be able to hold len bytes)
 * @param len # of bytes to read (0-65535)
 * @return error result.
 */
byte cpm_write(byte *buf, unsigned int len);

#endif /* CPM_H */
