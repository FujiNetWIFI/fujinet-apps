/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#ifndef DCB_H
#define DCB_H

typedef struct _dcb
{
  unsigned char status;
  void *buf;
  unsigned short len;
  unsigned long block;
  unsigned char unit;
  unsigned char reserved0;
  unsigned char reserved1;
  unsigned char reserved2;
  unsigned char reserved3;
  unsigned char reserved4;
  unsigned char reserved5;
  unsigned char dev;
  unsigned short max_len;
  unsigned char type;
  unsigned char dev_status;
} DCB;

#endif /* DCB_H */
