/**
 * @brief MS-DOS AppKey funcs
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl, v. 3
 */

#include <stdlib.h>
#include <stdbool.h>
#include <i86.h>
#include "../platform-specific/appkey.h"

#define FUJICMD_OPEN_APPKEY 0xDC
#define FUJICMD_WRITE_APPKEY 0xDE
#define FUJICMD_READ_APPKEY 0xDD

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
    return 0;
}

void read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
{
    return;
}

void write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char *data)
{
  return;
}

