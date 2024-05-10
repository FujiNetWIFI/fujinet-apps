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
  data.open.creator = creator_id;
  data.open.app = app_id;
  data.open.key = key_id;
  data.open.mode = open_mode;

  return fuji_appkey_open(&data.open);
}

void read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* destination)
{
  unsigned char read = 0;
  if (open_appkey(0, creator_id, app_id, key_id))
  {
    read = fuji_appkey_read(&data.read);

    if (read>0) {
        memcpy(destination, data.read.value, data.read.length);
        read = data.read.length;
    }
  }

  // Add string terminator at end
  destination[read] = 0;
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, const char *inputString)
{
  unsigned char appkey_error=0;
  if (open_appkey(1, creator_id, app_id, key_id))
  {
    strcpy((char *) data.write.value, inputString);  
    appkey_error = fuji_appkey_write(strlen(inputString), &data.write);
  }
  return appkey_error;
}

#endif /* __C64__ */