/**
 * @brief Appkey Routines
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <eos.h>
#include <string.h>
#include "appkey.h"

unsigned char appkey_read(unsigned int creator, unsigned char app, unsigned char key, char *buf)
{
  unsigned char r=0;
  
  struct
  {
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    char data[64];
  } a;

  a.cmd = ADAMNET_SEND_APPKEY_READ;
  a.creator = creator;
  a.app = app;
  a.key = key;

  r=eos_write_character_device(ADAMNET_FUJI_DEVICE_ID,(unsigned char *)a,sizeof(a));
  if (r > 0x80)
    return r;
  else
    return eos_read_character_device(ADAMNET_FUJI_DEVICE_ID,buf,MAX_APPKEY_LEN);
}

unsigned char appkey_write(unsigned int creator, unsigned char app, unsigned char key, char *buf)
{
  struct
  {
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    char data[64];
  } a;

  a.cmd = ADAMNET_SEND_APPKEY_WRITE;
  a.creator = creator;
  a.app = app;
  a.key = key;
  strncpy(a.data,buf,sizeof(a.data));

  return eos_write_character_device(ADAMNET_FUJI_DEVICE_ID,(unsigned char *)a,sizeof(a));
}
