/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

#include "sp.h"
#include <conio.h>
#include <apple2.h>
#include <peekpoke.h>

#define SP_CMD_STATUS 0
#define SP_CMD_READBLOCK 1
#define SP_CMD_WRITEBLOCK 2
#define SP_CMD_CONTROL 4
#define SP_STATUS_PARAM_COUNT 3
#define SP_CONTROL_PARAM_COUNT 3
#define SP_READBLOCK_PARAM_COUNT 3
#define SP_WRITEBLOCK_PARAM_COUNT 3

// extern globals:
uint8_t sp_payload[1024];
uint16_t sp_count;
uint8_t sp_dest;
uint8_t sp_error;

static uint8_t sp_cmdlist[10];
static uint8_t sp_cmdlist_low, sp_cmdlist_high;
static uint8_t sp_err, sp_rtn_low, sp_rtn_high;
static uint8_t sp_dispatch_low, sp_dispatch_high;

int8_t sp_status(uint16_t sp_dispatch, uint8_t dest, uint8_t statcode)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_STATUS_PARAM_COUNT; 
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = statcode;

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // store cmd list
  __asm__ volatile ("lda #%b", SP_CMD_STATUS);
  __asm__ volatile ("sta %g", spCmd); // store status command # 
  __asm__ volatile ("lda %v", sp_cmdlist_low);
  __asm__ volatile ("sta %g", spCmdListLow); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_high);
  __asm__ volatile ("sta %g", spCmdListHigh); // store status command #
  __asm__ volatile ("lda #%b", 0x20); // jsr opcode
  __asm__ volatile ("sta %g", spJmp);
  __asm__ volatile ("lda %v", sp_dispatch_low); // sp entry lsb
  __asm__ volatile ("sta %g", spDispatchLow);
  __asm__ volatile ("lda %v", sp_dispatch_high); // sp entry msb
  __asm__ volatile ("sta %g", spDispatchHigh);
spJmp:
  __asm__ volatile ("nop");
spDispatchLow:
  __asm__ volatile ("nop");
spDispatchHigh:
  __asm__ volatile ("nop");
spCmd:
  __asm__ volatile ("nop");
spCmdListLow:
  __asm__ volatile ("nop");
spCmdListHigh:
  __asm__ volatile ("nop");
  __asm__ volatile ("stx %v", sp_rtn_low);
  __asm__ volatile ("sty %v", sp_rtn_high);
  __asm__ volatile ("sta %v", sp_err);
  
  sp_count = ((uint16_t)sp_rtn_high << 8) | (uint16_t)sp_rtn_low;
  sp_error = sp_err;
  return sp_err;
}

int8_t sp_control(uint16_t sp_dispatch, uint8_t dest, uint8_t ctrlcode)
{
  sp_error = 0;
  // sp_dest = 5; // need to search
  // build the command list
  sp_cmdlist[0] = SP_CONTROL_PARAM_COUNT; 
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = ctrlcode;

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;

  // store cmd list
  __asm__ volatile ("lda #%b", SP_CMD_CONTROL);
  __asm__ volatile ("sta %g", spCmd); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_low);
  __asm__ volatile ("sta %g", spCmdListLow); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_high);
  __asm__ volatile ("sta %g", spCmdListHigh); // store status command #
  __asm__ volatile ("lda #%b", 0x20); // jsr opcode
  __asm__ volatile ("sta %g", spJmp);
  __asm__ volatile ("lda %v", sp_dispatch_low); // sp entry lsb
  __asm__ volatile ("sta %g", spDispatchLow);
  __asm__ volatile ("lda %v", sp_dispatch_high); // sp entry msb
  __asm__ volatile ("sta %g", spDispatchHigh);
spJmp:
  __asm__ volatile ("nop");
spDispatchLow:
  __asm__ volatile ("nop");
spDispatchHigh:
  __asm__ volatile ("nop");
spCmd:
  __asm__ volatile ("nop");
spCmdListLow:
  __asm__ volatile ("nop");
spCmdListHigh:
  __asm__ volatile ("nop");
  __asm__ volatile ("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

int8_t sp_readblock(uint16_t sp_dispatch, uint8_t dest, uint32_t blocknum)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_READBLOCK_PARAM_COUNT; 
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = blocknum & 0xFF;
  sp_cmdlist[5] = (blocknum >> 8) & 0xFF;
  sp_cmdlist[6] = (blocknum >> 16) & 0xFF;
  
  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;
  
  // store cmd list
  __asm__ volatile ("lda #%b", SP_CMD_READBLOCK);
  __asm__ volatile ("sta %g", spCmd); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_low);
  __asm__ volatile ("sta %g", spCmdListLow); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_high);
  __asm__ volatile ("sta %g", spCmdListHigh); // store status command #
  __asm__ volatile ("lda #%b", 0x20); // jsr opcode
  __asm__ volatile ("sta %g", spJmp);
  __asm__ volatile ("lda %v", sp_dispatch_low); // sp entry lsb
  __asm__ volatile ("sta %g", spDispatchLow);
  __asm__ volatile ("lda %v", sp_dispatch_high); // sp entry msb
  __asm__ volatile ("sta %g", spDispatchHigh);
spJmp:
  __asm__ volatile ("nop");
spDispatchLow:
  __asm__ volatile ("nop");
spDispatchHigh:
  __asm__ volatile ("nop");
spCmd:
  __asm__ volatile ("nop");
spCmdListLow:
  __asm__ volatile ("nop");
spCmdListHigh:
  __asm__ volatile ("nop");
  __asm__ volatile ("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

int8_t sp_writeblock(uint16_t sp_dispatch, uint8_t dest, uint32_t blocknum)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_WRITEBLOCK_PARAM_COUNT; 
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = blocknum & 0xFF;
  sp_cmdlist[5] = blocknum >> 8 & 0xFF;
  sp_cmdlist[6] = blocknum >> 16 & 0xFF;
  
  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;
  sp_dispatch_low = (uint8_t)((uint16_t)sp_dispatch & 0x00FF);
  sp_dispatch_high = (uint8_t)((uint16_t)sp_dispatch >> 8) & 0xFF;
  
  // store cmd list
  __asm__ volatile ("lda #%b", SP_CMD_WRITEBLOCK);
  __asm__ volatile ("sta %g", spCmd); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_low);
  __asm__ volatile ("sta %g", spCmdListLow); // store status command #
  __asm__ volatile ("lda %v", sp_cmdlist_high);
  __asm__ volatile ("sta %g", spCmdListHigh); // store status command #
  __asm__ volatile ("lda #%b", 0x20); // jsr opcode
  __asm__ volatile ("sta %g", spJmp);
  __asm__ volatile ("lda %v", sp_dispatch_low); // sp entry lsb
  __asm__ volatile ("sta %g", spDispatchLow);
  __asm__ volatile ("lda %v", sp_dispatch_high); // sp entry msb
  __asm__ volatile ("sta %g", spDispatchHigh);
spJmp:
  __asm__ volatile ("nop");
spDispatchLow:
  __asm__ volatile ("nop");
spDispatchHigh:
  __asm__ volatile ("nop");
spCmd:
  __asm__ volatile ("nop");
spCmdListLow:
  __asm__ volatile ("nop");
spCmdListHigh:
  __asm__ volatile ("nop");
  __asm__ volatile ("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/*
int8_t sp_find_fuji()
{
  // const char fuji[9] = "THE_FUJI";
  const char fuji[14] = "FUJINET_DISK_0";
  const uint8_t fuji_len = sizeof(fuji);
  int8_t err, num, i, j;

  err = sp_status(0x00, 0x00); // get number of devices
	if (err)
    return -err;
  num = sp_payload[0];
	num++;
	for (i = 1; i < num; i++)
	{
    //do
      err = sp_status(i, 0x03); // get DIB
    //while (err);
    if (sp_payload[4] == fuji_len)
    {
      for (j = 0; j < fuji_len; j++)
        if (fuji[j]!=sp_payload[5+j])
          return 0;
      sp_dest = i; // store the fuji unit #
      return i;
    }
	}
  return 0;
}
*/

/*
void sp_list_devs()
{
  int8_t err, num, i, j;

  err = sp_status(0x00, 0x00); // get number of devices
  num = sp_payload[0];
  num += 2;
  for (i = 1; i < num; i++)
  {
    cprintf("UNIT #%d NAME: ", i);
    err = sp_status(i, 0x03);
    for (j = 0; j < sp_payload[4]; j++)
      cputc(sp_payload[5 + j]);
    cputs("\r\n");
  }
  cgetc();
}
*/

/**
 * Apple // SmartPort routines for CC65
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

/**
 * Check for SmartPort presence
 * @return slot #, or 0xFF if not present.
 */
uint8_t sp_find_slot(void)
{
  uint8_t s=0;

  for (s=7; s > 0; s--)
    {
      uint16_t a = 0xc000 + (s * 0x100);
      if ((PEEK(a+1) == 0x20) &&
          (PEEK(a+3) == 0x00) &&
          (PEEK(a+5) == 0x03) &&
          (PEEK(a+7) == 0x00))
        return s;
    }

  // Did not find.
  return 0;
}

/**
 * Return dispatch address for Smartport slot.
 * @param s Slot # (1-7)
 * @return smartport dispatch address
 */
uint16_t sp_dispatch_address(uint8_t slot)
{
  uint16_t a = (slot * 0x100) + 0xC000;
  uint8_t j = PEEK(a+0xFF);

  return a + j + 3;
}

