#ifdef __APPLE2__
/**
 * @brief   Cbm App Key Functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#include "../fujinet-fuji.h"
#include <string.h>
#include <stdio.h>

static AppKeyDataBlock data;

/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return success code
 */
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, unsigned char key_id)
{ 
  static unsigned char appkey_error;
  printf("Opening..\n");
  data.open.creator = creator_id;
  data.open.app = app_id;
  data.open.key = key_id;
  data.open.mode = open_mode;
  data.open.reserved = 0x00;

  return fuji_appkey_open(&data.open);
}

unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* destination)
{
  static unsigned char read;
  if (open_appkey(0, creator_id, app_id, key_id) == 0)
  {
    destination[0] = 0;
  }
  
  read = fuji_appkey_read(&data.read);
  printf("Read %i\n", read);
  if (read>0) {  
      memcpy(destination, data.read.value, data.read.length);
      read = data.read.length;
      printf("Read %i bytes: %s\n", read, data.read.value);
  }

  // Add string terminator at end
  destination[read] = 0;
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, const char *inputString)
{
  static unsigned char appkey_error;
  if (open_appkey(1, creator_id, app_id, key_id) == 0)
    return 0;
  
  printf("Going to write: %s\n", inputString);
  
  strcpy((char *) data.write.value, inputString);
  
  printf("Writing %i chars: %s\n", strlen(inputString), &data.write);

  appkey_error = fuji_appkey_write(strlen(inputString), &data.write);
  printf("result: %i\n", appkey_error);
  return appkey_error;
}

#endif /* __C64__ */