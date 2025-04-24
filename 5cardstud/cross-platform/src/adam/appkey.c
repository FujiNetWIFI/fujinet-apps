/**
 * @brief Coleco Adam App Key Functions
 * @author Thomas Cherryhomes, Geoff Oltmans
 * @license gpl v. 3
 */

#ifdef __ADAM__
#include <eos.h>

#define ADAMNET_SEND_APPKEY_READ  0xDD
#define ADAMNET_SEND_APPKEY_WRITE 0xDE

#define MAX_APPKEY_LEN 64

#define ADAMNET_FUJI_DEVICE_ID 0x0F


/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return error code
 */
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, char key_id)
{
  //unused in adam implementation...
  return 0;
}

unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
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
  a.creator = creator_id;
  a.app = app_id;
  a.key = key_id;

  r=eos_write_character_device(ADAMNET_FUJI_DEVICE_ID,(unsigned char *)a,sizeof(a));
  if (r > 0x80)
    return r;
  else
    return eos_read_character_device(ADAMNET_FUJI_DEVICE_ID,data,MAX_APPKEY_LEN);
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, const char *data)
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
  a.creator = creator_id;
  a.app = app_id;
  a.key = key_id;
  strncpy(a.data,data,sizeof(a.data));

  return eos_write_character_device(ADAMNET_FUJI_DEVICE_ID,(unsigned char *)a,sizeof(a));
}

#endif /* __ADAM__ */
