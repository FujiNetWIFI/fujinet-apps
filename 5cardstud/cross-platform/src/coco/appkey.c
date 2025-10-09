#ifdef _CMOC_VERSION_
/**
 * @brief   Cbm App Key Functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#include <coco.h>
#include "../fujinet-fuji.h"
//#include <string.h>
//#include <stdio.h>

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern uint8_t ak_appkey_size;

// Temp override until fujinet-lib ak_appkey_size enum fix
void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
{
	ak_appkey_size = keysize;
	ak_app_id = app_id;
	ak_creator_id = creator_id;
}

void read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint8_t* destination) {
  uint16_t read = 0;

  fuji_set_appkey_details(creator_id, app_id, DEFAULT);
  if (!fuji_read_appkey(key_id, &read, destination))
    read=0;

  // Add string terminator at end
  destination[read] = 0;
}

void write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, const char *inputString)
{
  fuji_set_appkey_details(creator_id, app_id, DEFAULT);
  fuji_write_appkey(key_id, strlen(inputString), (uint8_t *)inputString);
}

#endif
