/*
  app_key.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module allows storing and fetching display settings
  (Red, Green, and Blue hues, and APAC mode Luminence)
  on the FujiNet itself (note: requires an SD card at this time;
  see https://github.com/FujiNetWIFI/fujinet-platformio/wiki/SIO-Command-%24DC-Open-App-Key)

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-05-03 - 2021-05-03
*/

#include <atari.h>
#include "app_key.h"
#include "sio.h"
#include "rgb.h"

#define APP_KEY_SIO_DEVICEID 0x70 /* FujiNet App Key */
#define APP_KEY_SIO_UNIT 1
#define APP_KEY_SIO_TIMEOUT 15
#define APP_KEY_SIO_CMD_OPEN 0xDC
#define APP_KEY_SIO_CMD_APPKEY_READ 0xDD
#define APP_KEY_SIO_CMD_APPKEY_WRITE 0xDE

/* (From https://github.com/FujiNetWIFI/fujinet-apps/blob/master/appkey-sample/src/main.c
   by Oscar Fowler) */
typedef union {
  struct
  {
    unsigned int creator;
    unsigned char app;
    unsigned char key;
    unsigned char mode;
    unsigned char reserved;
  } open;
  struct
  {
    unsigned int length;
    unsigned char value[MAX_APPKEY_LEN];
  } read;
  struct
  {
    unsigned char value[MAX_APPKEY_LEN];
  } write;
} datablock;

static datablock app_key_data;

/**
 * Open our app key store on the FujiNet
 * (used by the read & write routines, below).
 *
 * @param byte mode - 0=read, 1=write
 * @return byte SIO status
 */
unsigned char open_appkey(unsigned char mode) {
  app_key_data.open.creator = CREATOR_ID;
  app_key_data.open.app = APP_ID;
  app_key_data.open.key = KEY_ID;
  app_key_data.open.mode = mode;
  app_key_data.open.reserved = 0x00;

  OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
  OS.dcb.dunit = APP_KEY_SIO_UNIT;
  OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;

  OS.dcb.dcomnd = APP_KEY_SIO_CMD_OPEN;
  OS.dcb.dstats = SIO_WRITE;
  OS.dcb.dbuf = &app_key_data;
  OS.dcb.dbyt = sizeof(app_key_data.open);
  OS.dcb.daux = 0;

  siov();

  return OS.dcb.dstats;
}

/**
 * Read APOD settings from FujiNet app key store.
 *
 * Note: Fails silently.
 */
void read_settings(void) {
  unsigned char dstats;

  dstats = open_appkey(0);
  if (dstats == 1 /* operation complete (no errors) */) {
    OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
    OS.dcb.dunit = APP_KEY_SIO_UNIT;
    OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;
  
    OS.dcb.dcomnd = APP_KEY_SIO_CMD_APPKEY_READ;
    OS.dcb.dstats = SIO_READ;
    OS.dcb.dbuf = &app_key_data;
    OS.dcb.dbyt = sizeof(app_key_data.read);
    OS.dcb.daux = 0;
  
    siov();

    if (OS.dcb.dstats == 1 /* operation complete (no errors) */) {
      rgb_red = app_key_data.write.value[0]; 
      rgb_grn = app_key_data.write.value[1]; 
      rgb_blu = app_key_data.write.value[2]; 
      apac_lum = app_key_data.write.value[3];
    }
  }
}

/**
 * Write APOD settings to FujiNet app key store.
 *
 * @return bool whether it seemed to succeed
 */
unsigned char write_settings(void) {
  unsigned char dstats;

  dstats = open_appkey(1);
  if (dstats == 1 /* operation complete (no errors) */) {
    app_key_data.write.value[0] = rgb_red;
    app_key_data.write.value[1] = rgb_grn;
    app_key_data.write.value[2] = rgb_blu;
    app_key_data.write.value[3] = apac_lum;

    OS.dcb.ddevic = APP_KEY_SIO_DEVICEID;
    OS.dcb.dunit = APP_KEY_SIO_UNIT;
    OS.dcb.dtimlo = APP_KEY_SIO_TIMEOUT;
  
    OS.dcb.dcomnd = APP_KEY_SIO_CMD_APPKEY_WRITE;
    OS.dcb.dstats = SIO_WRITE;
    OS.dcb.dbuf = &app_key_data.write;
    OS.dcb.dbyt = sizeof(app_key_data.write);
    OS.dcb.daux = 0;
  
    siov();

    if (OS.dcb.dstats == 1 /* operation complete (no errors) */) {
      return 1;
    }
  }
  return 0;
}


