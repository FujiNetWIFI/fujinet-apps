/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

#ifndef SP_H
#define SP_H

#include <stdint.h>

extern uint8_t sp_payload[1024];
extern uint16_t sp_count, sp_dispatch;
extern uint8_t sp_dest;
extern uint8_t sp_error;

int8_t sp_status(uint8_t dest, uint8_t statcode);
int8_t sp_control(uint8_t dest, uint8_t ctrlcode);
int8_t sp_open(uint8_t dest);
int8_t sp_close(uint8_t dest);
int8_t sp_read(uint8_t dest, uint16_t len);
int8_t sp_write(uint8_t dest, uint16_t len);

int8_t sp_find_fuji(void);
int8_t sp_find_network(void);
int8_t sp_find_clock(void);
int8_t sp_find_modem();
int8_t sp_find_cpm();
int8_t sp_find_printer();

uint8_t  sp_find_slot(void);
uint16_t sp_dispatch_address(uint8_t slot);
uint8_t  sp_init(void);

void     sp_list_devs();
int16_t  sp_bytes_waiting(uint8_t dest);
int8_t   sp_is_connected(uint8_t dest);
uint8_t  sp_accept(uint8_t dest);

#define SP_ERR_OK (0x00)
#define SP_ERR_BAD_CMD (0x01)
#define SP_ERR_BAD_PCNT (0x02)      //    ; BAD CALL PARAMETER COUNT
#define SP_ERR_BUS_ERR (0x06)       //    ; bus error in IWM chip
#define SP_ERR_BAD_UNIT (0x11)      //    ; UNIT NUMBER $00 WAS USED
#define SP_ERR_BAD_CTRL (0x21)      //    ; CTRL OR STATUS CODE WAS NOT SUPPORTED
#define SP_ERR_BAD_CTRL_PARM (0x22) //    ; CTRL PARAMTER LIST CONTAINS INVALID INFO
#define SP_ERR_IO_ERROR (0x27)      //    ; CAN'T ACCESS DEVICE OR DEVICE ERROR
#define SP_ERR_NO_DRIVE (0x28)      //    ; DEVICE IS NOT CONNECTED
#define SP_ERR_NO_WRITE (0x2B)      //    ; MEDIUM IS WRITE PROTECTED
#define SP_ERR_BAD_BLOCK (0x2D)     //    ; BLOCK NUMBER IS OUTSIDE OF RANGE
#define SP_ERR_DISK_SW (0x2E)       //    ; DISK SWITCH TOOK PLACE
#define SP_ERR_OFFLINE (0x2F)       //    ; DEVICE OFFLINE OR NO DISK IN DRIVE
#define SP_ERR_DEV_SPEC0 (0x30)     //    ; DEVICE SPECIFIC ERRORS
#define SP_ERR_DEV_SPECF (0x3F)     //    ; DEVICE SPECIFIC ERRORS
// SP_ERR_RESERVED $40-$4F
#define SP_ERR_NON_FATAL50 (0x50)   //    ; DEVICE SPECIFIC WARNING
#define SP_ERR_NON_FATAL7F (0x7F)   //    ; DEVICE SPECIFIC WARNING

#endif /* SP_H */
