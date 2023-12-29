#ifdef __C64__
/**
 * @brief   Cbm App Key Functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#include <string.h>
#include <cbm.h>
#include "../appkey.h"

#define FUJICMD_WRITE_APPKEY "\xDE"
#define FUJICMD_READ_APPKEY "\xDD"
#define FUJICMD_OPEN_APPKEY "\xDC"
#define FUJICMD_CLOSE_APPKEY "\xDB"

#define DEV 30  // The FujiNet device #

appkeyblock appkey; // The appkey (see appkey.h)
unsigned char appkey_error;

// Defint "EMULATE_FUJINET" to use emulator bridge sinstead of Meatloaf/FujiNet hardware

#ifdef EMULATE_FUJINET
  #include <stdio.h>
  char appkey_vice_file[32];
  #define LFN 1
  #define APPKEY_VALUE (const char *)appkey.write.value
#else
  #define LFN 15
  #define APPKEY_VALUE (const char *)appkey.read.value
#endif

/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return error code
 */
unsigned char open_appkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, unsigned char key_id)
{  
  #ifdef EMULATE_FUJINET
    sprintf(appkey_vice_file,"ak-%x-%i-%i", creator_id, app_id, key_id);
    return 0;
  #endif

  appkey.open.creator = creator_id;
  appkey.open.app = app_id;
  appkey.open.key = key_id;
  appkey.open.mode = open_mode;
  appkey.open.reserved = 0x00;

  appkey_error = cbm_open(LFN,15,15,FUJICMD_OPEN_APPKEY);
  if (appkey_error == 0) {
    cbm_write(LFN, &appkey, sizeof(appkey));
    cbm_close(LFN);
  }
  return appkey_error;
}



unsigned char read_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char* data)
{
  int read;
  
  // Set output as empty string
  data[0]=0;
  
  appkey_error = open_appkey(APPKEY_READ,creator_id,app_id,key_id);
  if (appkey_error)
    return appkey_error;
  
  #ifdef EMULATE_FUJINET
    appkey_error = cbm_open(LFN,DEV,0,appkey_vice_file);
  #else
    appkey_error = cbm_open(LFN,15,15,FUJICMD_READ_APPKEY);
  #endif

  if (appkey_error==0) {
    read = cbm_read(LFN, &appkey, sizeof(appkey.read));
    cbm_close(LFN);

    if (read>=2) {
      strcpy(data, APPKEY_VALUE);
    }
    
    // Future: may need to call close_appkey
  }

  return appkey_error;
}

unsigned char write_appkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data)
{
  appkey_error = open_appkey(APPKEY_WRITE,creator_id,app_id,key_id);
  if (appkey_error)
    return appkey_error;

  #ifdef EMULATE_FUJINET
    appkey_error = cbm_open(LFN,DEV,1,appkey_vice_file);
  #else
    appkey_error = cbm_open(LFN,15,15,FUJICMD_WRITE_APPKEY);
  #endif


  if (appkey_error==0) {
    strcpy(appkey.write.value, data);

    appkey_error = cbm_write(LFN, &appkey.write, sizeof(appkey.write));
    cbm_close(LFN);

    // Future: may need to call close_appkey
  }

  return appkey_error;
}

#endif /* __C64__ */