/**
 * #FujiNet Adam AppKey test
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <conio.h>
#include <msx.h>
#include <eos.h>

#define ADAMNET_FUJI_DEVICE_ID 0x0F

#define TEST_CREATOR_ID 0x0100
#define TEST_APP_ID 0x1F
#define TEST_KEY_ID 0xCC

#define ADAMNET_SEND_APPKEY_READ  0xDD
#define ADAMNET_SEND_APPKEY_WRITE 0xDE

#define MAX_APPKEY_LEN 64

unsigned char response[1024];

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
    return eos_read_character_device(ADAMNET_FUJI_DEVICE_ID,response,sizeof(response));
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

void main(void)
{
  msx_set_mode(0);
  printf("\r\nAPPKEY TEST\n\n");

  printf("%7s:%04X\n%7s:%02X\n%7s:%02X\n\n","CREATOR",TEST_CREATOR_ID,"APP",TEST_APP_ID,"KEY",TEST_KEY_ID);
  
  while(1)
    {
      printf("appkey_read returned %02xH\n\n",appkey_read(TEST_CREATOR_ID,TEST_APP_ID,TEST_KEY_ID,response));
      printf("CURRENT APPKEY VALUE:\n%s\n\n",response);
      printf("NEW APPKEY VALUE?\n");
      cgets(response);
      printf("appkey_write returned %02xH\n\n",appkey_write(TEST_CREATOR_ID,TEST_APP_ID,TEST_KEY_ID,response));
    }
}
