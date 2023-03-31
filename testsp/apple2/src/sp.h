/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

#ifndef SP_H
#define SP_H

#include <stdint.h>

extern uint8_t sp_payload[1024];
extern uint16_t sp_count;
extern uint8_t sp_dest;
extern uint8_t sp_error;

int8_t sp_status(uint16_t sp_dispatch, uint8_t dest, uint8_t statcode);
int8_t sp_control(uint16_t sp_dispatch, uint8_t dest, uint8_t ctrlcode);
int8_t sp_readblock(uint16_t sp_dispatch, uint8_t dest, uint32_t blocknum);
int8_t sp_writeblock(uint16_t sp_dispatch, uint8_t dest, uint32_t blocknum);
//int8_t sp_find_fuji(void);
//uint8_t sp_find_slot(void);
uint16_t sp_dispatch_address(uint8_t slot);

void sp_list_devs();

#endif /* SP_H */


