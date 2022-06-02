/**
 *  for Atari Lynx 
 *
 * @brief I/O routines
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 */

#ifndef FUJINET_H
#define FUJINET_H

unsigned char fujinet_init();
unsigned char fujinet_done();
unsigned char fujinet_send(unsigned char dev, unsigned char *buf, unsigned short len);
unsigned char fujinet_recv(unsigned char dev, unsigned char *buf, unsigned short *len);

#endif /* FUJINET_H */
