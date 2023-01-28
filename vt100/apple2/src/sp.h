/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

#ifndef SP_H
#define SP_H

#include <stdint.h>

// see page 81-82 in Apple IIc ROM reference and Table 7-5 in IIgs firmware ref
#define SP_ERR_NOERROR 0x00    // no error
#define SP_ERR_BADCMD 0x01     // invalid command
#define SP_ERR_BUSERR 0x06     // communications error
#define SP_ERR_BADCTL 0x21     // invalid status or control code
#define SP_ERR_BADCTLPARM 0x22 // invalid parameter list
#define SP_ERR_IOERROR 0x27    // i/o error on device side
#define SP_ERR_NODRIVE 0x28    // no device connected
#define SP_ERR_NOWRITE 0x2b    // disk write protected
#define SP_ERR_BADBLOCK 0x2d   // invalid block number
#define SP_ERR_DISKSW 0x2e     // media has been swapped - extended calls only
#define SP_ERR_OFFLINE 0x2f    // device offline or no disk in drive

extern uint8_t sp_payload[2048];
extern uint16_t sp_count, sp_dispatch;
extern uint8_t sp_dest;
extern uint8_t sp_error;

int8_t sp_status(uint8_t dest, uint8_t statcode);
int8_t sp_control(uint8_t dest, uint8_t ctrlcode);
int8_t sp_open(uint8_t dest);
int8_t sp_close(uint8_t dest);
int8_t sp_read(uint8_t dest, uint16_t len);
int8_t sp_write(uint8_t dest, uint16_t len);
int8_t sp_find_cpm(void);
uint8_t sp_find_slot(void);
uint16_t sp_dispatch_address(uint8_t slot);
void sp_init(void);

#endif /* SP_H */

