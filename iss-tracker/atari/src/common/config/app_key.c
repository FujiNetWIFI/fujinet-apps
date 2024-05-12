#include <atari.h>
#include <stdbool.h>
#include <stdint.h>

#include "app_key.h"
#include "colors.h"
#include "fujinet-fuji.h"

// max data a key can contain. The key only contains what we write to it, but we should allocate enough space for full key
static char key_data[64];

/**
 * Read ISS Tracker settings from FujiNet app key store.
 *
 * @return success status (true = success)
 */
bool read_settings(void) {
  uint16_t read_count = 0;

  fuji_set_appkey_details(CREATOR_ID, APP_ID, DEFAULT);
  if (!fuji_read_appkey(KEY_ID, &read_count, (uint8_t *) &key_data[0])) {
    return false;
  }
  if (read_count != 4) {
    return false;
  }

  color0 = key_data[0];
  color1 = key_data[1];
  color2 = key_data[2];
  color4 = key_data[3];

  return true;
}

/**
 * Write ISS Tracker settings to FujiNet app key store.
 *
 * @return success status (true = success)
 */
bool write_settings(void) {
  bool is_error;

  key_data[0] = color0;
  key_data[1] = color1;
  key_data[2] = color2;
  key_data[3] = color4;

  fuji_set_appkey_details(CREATOR_ID, APP_ID, DEFAULT);
  return fuji_write_appkey(KEY_ID, 4, (uint8_t *) &key_data[0]);

}
