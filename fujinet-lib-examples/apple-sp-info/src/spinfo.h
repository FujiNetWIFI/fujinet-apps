#ifndef TESTSP2_H
#define TESTSP2_H

#include <stdint.h>

void sp_info();

extern uint8_t sp_payload[];
extern uint8_t sp_dispatch_address[2];
extern uint16_t sp_count;

int8_t sp_status(uint8_t dest, uint8_t statcode);

#endif