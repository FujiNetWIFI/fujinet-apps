/**
 * @brief   High Level IO functions
 * @author  Eric Carr
 * @license gpl v. 3
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  unsigned char key;
  bool trigger;
  int8_t dirX;
  int8_t dirY;
} InputStruct;

extern InputStruct input;

uint16_t read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, char* destination);
void write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id,  uint16_t count, char *data);

void readCommonInput();
void inputField(uint8_t x, uint8_t y, uint8_t max, char* buffer);

#endif /* IO_H */
