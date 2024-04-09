#include <atari.h>
#include <stdbool.h>

#include "app_key.h"
#include "colors.h"
#include "fujinet-fuji.h"

static AppKeyDataBlock data;

/**
 * Open our app key store on the FujiNet
 * (used by the read & write routines, below).
 *
 * @param byte mode - 0=read, 1=write
 * @return fujinet ERROR status, true if there is an error
 */
bool open_appkey(unsigned char mode) {
  data.open.creator = CREATOR_ID;
  data.open.app = APP_ID;
  data.open.key = KEY_ID;
  data.open.mode = mode;
  data.open.reserved = 0x00;

  return fuji_appkey_open(&data.open);
}

/**
 * Read ISS Tracker settings from FujiNet app key store.
 *
 * Note: Fails silently.
 */
void read_settings(void) {
  bool is_error;

  is_error = open_appkey(0);
  if (is_error) {
    return;
  }

  is_error = fuji_appkey_read(&data.read);
  if (is_error) {
    return;
  }

  color0 = data.read.value[0];
  color1 = data.read.value[1];
  color2 = data.read.value[2];
  color4 = data.read.value[3];
}

/**
 * Write ISS Tracker settings to FujiNet app key store.
 *
 * @return bool whether it seemed to succeed
 */
unsigned char write_settings(void) {
  bool is_error;

  is_error = open_appkey(0);
  if (is_error) {
    return false;
  }

  data.write.value[0] = color0;
  data.write.value[1] = color1;
  data.write.value[2] = color2;
  data.write.value[3] = color4;

  is_error = fuji_appkey_write(4, &data.write);
  return !is_error;

}
