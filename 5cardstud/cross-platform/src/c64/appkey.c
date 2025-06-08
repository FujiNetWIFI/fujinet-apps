#ifdef __C64__

/**
 * @brief   Cbm App Key Functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <cbm.h>
#include "../platform-specific/appkey.h"

#define FUJICMD_OPEN_APPKEY 0xDC
#define FUJICMD_WRITE_APPKEY 0xDE
#define FUJICMD_READ_APPKEY 0xDD

#define LFN 15

typedef struct {
  unsigned char command;
  appkeyblock payload; // The appkey (see appkey.h)
} appkeyC64;

appkeyC64 appkey;

#ifdef USE_VICE_EMULATOR
  #define DEV 11
  #define SEC_NUM 0
  #define OPEN_APPKEY_PREFIX "appkey-"
  #define OPEN_APPKEY_DELIM "-"
#else
  #define DEV 30  // The FujiNet device #
  #define SEC_NUM 15
  #define OPEN_APPKEY_PREFIX "openappkey:"
  #define OPEN_APPKEY_DELIM ","
#endif

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
  static unsigned char appkey_error;

  #ifdef USE_VICE_EMULATOR  
  
  if (open_mode == APPKEY_WRITE)
    strcpy(appkey.payload.write.value, "s:");
  else
    strcpy(appkey.payload.write.value, "");

  itoa(creator_id, &appkey.payload.write.value[strlen(appkey.payload.write.value)], 16);

  strcat(appkey.payload.write.value, OPEN_APPKEY_DELIM);
  itoa(app_id, &appkey.payload.write.value[strlen(appkey.payload.write.value)], 16);
  
  strcat(appkey.payload.write.value, OPEN_APPKEY_DELIM);
  itoa(key_id, &appkey.payload.write.value[strlen(appkey.payload.write.value)], 16);

  strcat(appkey.payload.write.value, ".key");

  // strcat(appkey.payload.write.value, OPEN_APPKEY_DELIM);
  // itoa(open_mode, &appkey.payload.write.value[strlen(appkey.payload.write.value)], 10);

  // strcat(appkey.payload.write.value, OPEN_APPKEY_DELIM);
  // strcat(appkey.payload.write.value, "0");

  if (open_mode == APPKEY_WRITE) {
    appkey_error = cbm_open(LFN, DEV, 15, appkey.payload.write.value);
    cbm_close(LFN);

    appkey_error = cbm_open(LFN,DEV,open_mode,&appkey.payload.write.value[2]);
  } else {
    appkey_error = cbm_open(LFN,DEV,open_mode,&appkey.payload.write);
  }
  #else
    appkey.command = FUJICMD_OPEN_APPKEY;
    appkey.payload.open.creator = creator_id;
    appkey.payload.open.app = app_id;
    appkey.payload.open.key = key_id;
    appkey.payload.open.mode = open_mode;
    appkey.payload.open.reserved = 0;
    cbm_open(LFN,DEV,SEC_NUM,"");
    cbm_write(LFN, &appkey, sizeof(appkey));
  #endif
  
  #ifdef USE_VICE_EMULATOR
    return appkey_error;
  #endif

  cbm_close(LFN);
  
  return appkey_error;
}



void read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char* data)
{
  static unsigned char appkey_error;
  static int read;
  static unsigned int command = FUJICMD_READ_APPKEY;

  // Set output as empty string
  data[0]=0;

  if (appkey_error = open_appkey(APPKEY_READ, creator_id, app_id, key_id))
    return;
  
  #ifndef USE_VICE_EMULATOR
    cbm_close(LFN);
    appkey_error = cbm_open(LFN,DEV,SEC_NUM,(const char *)&command);
  #endif

  if (!appkey_error) {
    read = cbm_read(LFN, &appkey.payload, sizeof(appkey.payload.read));

    if (read>1 && appkey.payload.read.length <= MAX_APPKEY_LEN) {  
        memcpy(data, appkey.payload.read.value, appkey.payload.read.length);
        data[appkey.payload.read.length] = 0;
    } else {
        appkey_error = 64;
    }

    // Future: may need to call close_appkey
  }

  cbm_close(LFN);
  return;
}

void write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, char *data)
{
  static unsigned char appkey_error;
  static unsigned char len;

  if (appkey_error = open_appkey(APPKEY_WRITE, creator_id, app_id, key_id))
    return;

  len = strlen(data);
  if (len > MAX_APPKEY_LEN)
    len = MAX_APPKEY_LEN;

  memset(&appkey.payload, 0, sizeof(appkey.payload.read));
  appkey.payload.read.length = len;
  memcpy(&appkey.payload.read.value, data, appkey.payload.read.length);

  #ifdef USE_VICE_EMULATOR
    cbm_write(LFN, &appkey.payload.read, sizeof(appkey.payload.read));
  #else
    appkey.command = FUJICMD_WRITE_APPKEY;
    cbm_open(LFN,DEV,SEC_NUM,"");
    cbm_write(LFN, &appkey, sizeof(appkey));
  #endif

  cbm_close(LFN);

  // Future: may need to call close_appkey

  return;
}

#endif /* __C64__ */
