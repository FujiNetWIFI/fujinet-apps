#ifdef BUILD_APPLE2
/**
 * FujiNet CONFIG for #Apple2
 *
 * SmartPort MLI Routines
 */

#ifdef __INTELLISENSE__
// 18, expect closing parenthses - needed to use cc65 inline asm command with agruments.
#pragma diag_suppress 18
#endif

#include "sp.h"
#include <conio.h>
#include <stdio.h>
#include <apple2.h>
#include <peekpoke.h>

#define SP_CMD_STATUS 0
#define SP_CMD_CONTROL 4
#define SP_CMD_OPEN 6
#define SP_CMD_CLOSE 7
#define SP_CMD_READ 8
#define SP_CMD_WRITE 9
#define SP_STATUS_PARAM_COUNT 3
#define SP_CONTROL_PARAM_COUNT 3
#define SP_OPEN_PARAM_COUNT 1
#define SP_CLOSE_PARAM_COUNT 1
#define SP_READ_PARAM_COUNT 4
#define SP_WRITE_PARAM_COUNT 4

// extern globals:
uint8_t sp_payload[1024];
uint16_t sp_count;
uint8_t sp_dest;
uint16_t sp_dispatch = 0;

uint8_t sp_error;

static uint8_t sp_cmdlist[10];
static uint8_t sp_cmdlist_low, sp_cmdlist_high;
static uint8_t sp_err, sp_rtn_low, sp_rtn_high;
static uint8_t sp_dispatch_addr_low, sp_dispatch_addr_high;

/*************************************************************/

/**
 * Returns the status of a specified device
 * @param dest unit to query
 * @param statcode type of status code 
 * @return 0: no error occured, otherwise the smartport error code.
 */
int8_t sp_status(uint8_t dest, uint8_t statcode)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_STATUS_PARAM_COUNT;
  sp_cmdlist[1] = dest; 
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = statcode;

  // store cmd list

  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);

  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************

  __asm__ volatile("lda #%b", SP_CMD_STATUS);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("stx %v", sp_rtn_low);
  __asm__ volatile("sty %v", sp_rtn_high);
  __asm__ volatile("sta %v", sp_err);

  sp_count = ((uint16_t)sp_rtn_high << 8) | (uint16_t)sp_rtn_low;
  sp_error = sp_err;
  return sp_err;
}

/*****************************************************/

/**
 * Issues a control to the specified device.
 * NOTE: values within sp_payload must be set prior to
 * calling this routine.
 * @param dest unit to query
 * @param ctrlcode type of status code
 * @return 0: no error occured, otherwise the smartport error code.
 */
int8_t sp_control(uint8_t dest, uint8_t ctrlcode)
{
  sp_error = 0;
  // sp_dest = 5; // need to search
  // build the command list
  sp_cmdlist[0] = SP_CONTROL_PARAM_COUNT;
  sp_cmdlist[1] = dest; 
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = ctrlcode;

  // store cmd list

  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);

  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************

  __asm__ volatile("lda #%b", SP_CMD_CONTROL);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/*************************************************/

/**
 * Open smartport device
 * @param dest unit to query
 * @return 0: no error occured, otherwise the smartport error code.
 */
int8_t sp_open(uint8_t dest)
{
  sp_error = 0;
  // sp_dest = 5; // need to search
  // build the command list
  sp_cmdlist[0] = SP_OPEN_PARAM_COUNT;
  sp_cmdlist[1] = dest; 

  // store cmd list

  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);

  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************

  __asm__ volatile("lda #%b", SP_CMD_OPEN);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/*************************************************/

/**
 * Close the specified device
 * @param dest unit to query
 * @return 0: no error occured, otherwise the smartport error code.
 */
int8_t sp_close(uint8_t dest)
{
  sp_error = 0;
  // sp_dest = 5; // need to search
  // build the command list
  sp_cmdlist[0] = SP_CLOSE_PARAM_COUNT;
  sp_cmdlist[1] = dest; 

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;

  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);

  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************

  // store cmd list
  __asm__ volatile("lda #%b", SP_CMD_CLOSE);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/******************************************************/

/**
 * Read the specified number of bytes from the specified device
 * and store them in the sp_payload array
 * @param dest unit to query
 * @param len the number of bytes to collect
 * @return 0: no error occured, otherwise the smartport error code.
 */

int8_t sp_read(uint8_t dest, uint16_t len)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_READ_PARAM_COUNT;
  sp_cmdlist[1] = dest; 
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = (uint8_t)(len & 0x00FF);
  sp_cmdlist[5] = (uint8_t)((len >> 8) & 0xFF);

  // store cmd list
  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);

  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************

  __asm__ volatile("lda #%b", SP_CMD_READ);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/*
int8_t sp_read(uint8_t dest, uint16_t len)
{
  sp_error = 0;
  // sp_dest = 5; // need to search
  // build the command list
  sp_cmdlist[0] = SP_READ_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = len & 0xFF;
  sp_cmdlist[5] = len >> 8;

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;

  // store cmd list
  __asm__ volatile("lda #%b", SP_CMD_READ);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

  __asm__ volatile("jsr $C50D"); // to do - find entry point and used it instead of hardcoded address
spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

*/



/***************************************************************/
/**
 * Write the specified bytes from the sp_payload array to the specified device
 * @param dest unit to query
 * @param len number of bytes to write
 * @return 0: no error occured, otherwise the smartport error code.
 */

int8_t sp_write(uint8_t dest, uint16_t len)
{
  sp_error = 0;

  /*
    LDA #SP_WRITE_PARAM_COUNT
    STA CMD_LIST                    ; PARAMETER COUNT
    STX CMD_LIST+1                  ; DESTINATION DEVICE
    LDA SP_PAYLOAD_ADDR
    STA CMD_LIST+2                  ; DATA BUFFER
    LDA SP_PAYLOAD_ADDR+1
    STY CMD_LIST+4                  ; Y=LENGTH LO
    LDA #$00
    STA CMD_LIST+5                  ; LENGTH HI
    STA CMD_LIST+6                  ; ADDRESS POINTER LOW
    STA CMD_LIST+7                  ; ADDRESS POINTER MID
    STA CMD_LIST+8                  ; ADDRESS POINTER HI
  */

  // build the command list
  sp_cmdlist[0] = SP_WRITE_PARAM_COUNT;
  sp_cmdlist[1] = dest; 
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = (uint8_t)(len & 0x00FF);
  sp_cmdlist[5] = (uint8_t)((len >> 8) & 0xFF);
  sp_cmdlist[6] = 0;
  sp_cmdlist[7] = 0;
  sp_cmdlist[8] = 0;

  // ********* PRELOADER ********************
  __asm__ volatile("lda #$20"); // JSR
  __asm__ volatile("sta %g", spJsr);
  __asm__ volatile("lda %v", sp_dispatch_addr_low);
  __asm__ volatile("sta %g", var_addr_low1);
  __asm__ volatile("sta %g", var_addr_low2);
  __asm__ volatile("lda %v", sp_dispatch_addr_high);
  __asm__ volatile("sta %g", var_addr_high1);
  __asm__ volatile("sta %g", var_addr_high2);

  __asm__ volatile("lda #$BD"); // lda addr,x
  __asm__ volatile("sta %g", ldax1);
  __asm__ volatile("sta %g", ldax2);


// first we have to get the addresses set so
// we find the address where the dispatch address
// is stored, then we can get the actual values
// using this address
// 
//                  LDX #$00
//                  LDA &sp_dispatch low
//                  STA ..
//                  LDA &sp_dispatch high
//                  STA ..
//
//                  LDX #$00
//  ..              LDA *sp_dispatch,X
//                  STA dispatch address low
//                  INX
//  ..              LDA *sp_dispatch,X
//                  STA dispatch address high



  __asm__ volatile("ldx #$00");
ldax1:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low1:
  __asm__ volatile("nop");
var_addr_high1:
  __asm__ volatile("nop");

  __asm__ volatile("sta %g", spDispatch_addr_low);

  __asm__ volatile("inx");

ldax2:
  __asm__ volatile("nop"); // $BD lda addr,x
var_addr_low2:
  __asm__ volatile("nop");
var_addr_high2:
  __asm__ volatile("nop");
  __asm__ volatile("sta %g", spDispatch_addr_high);

  // ********* PRELOADER END ********************


  __asm__ volatile("lda #%b", SP_CMD_WRITE);
  __asm__ volatile("sta %g", spCmd);              // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #
    
spJsr:
  __asm__ volatile("nop"); // JSR
spDispatch_addr_low:
  __asm__ volatile("nop");
spDispatch_addr_high:
  __asm__ volatile("nop");

spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}

/*
int8_t sp_write(uint8_t dest, uint16_t len)
{
  sp_error = 0;
  // build the command list
  sp_cmdlist[0] = SP_READ_PARAM_COUNT;
  sp_cmdlist[1] = dest; // set before calling sp_status();
  sp_cmdlist[2] = (uint8_t)((uint16_t)&sp_payload & 0x00FF);
  sp_cmdlist[3] = (uint8_t)((uint16_t)&sp_payload >> 8) & 0xFF;
  sp_cmdlist[4] = len & 0xFF;
  sp_cmdlist[5] = len >> 8;

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;

  // store cmd list
  __asm__ volatile("lda #%b", SP_CMD_WRITE);
  __asm__ volatile("sta %g", spCmd); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_low);
  __asm__ volatile("sta %g", spCmdListLow); // store status command #
  __asm__ volatile("lda %v", sp_cmdlist_high);
  __asm__ volatile("sta %g", spCmdListHigh); // store status command #

  __asm__ volatile("jsr $C50D"); // to do - find entry point and used it instead of hardcoded address
spCmd:
  __asm__ volatile("nop");
spCmdListLow:
  __asm__ volatile("nop");
spCmdListHigh:
  __asm__ volatile("nop");
  __asm__ volatile("sta %v", sp_err);
  sp_error = sp_err;
  return sp_err;
}
*/

/**********************/
/**** FIND DEVICES ****/
/**********************/

int8_t sp_find_device(const char *name, int8_t name_len)
{
  int8_t err, num, i, j;
  int8_t found = 0;

  name_len--; // no need for null

  err = sp_status(0x00, 0x00); // get number of devices

  if (err)
    return -err;

  num = sp_payload[0];
  num++;

  for (i = 1; i < num; i++)
  {
    err = sp_status(i, 0x03); // get DIB

    // if we get an error, don't process this info
    if (err)
      continue;

    // if it's not the same length, no need to
    // check further

    if (sp_payload[4] == name_len)
    {
      found = (int8_t)1;
      for (j = 0; j < name_len; j++)
      {
        if (name[j] != sp_payload[5 + j])
        {
          // try the next device
          found = (int8_t)0;
          break;
        }
      } // for

      // No need to continue if found
      if (found)
        break;

    } // if len

  } // for

  if (!found)
    return (int8_t)0;
  else
    return (int8_t)i;
}

int8_t sp_find_fuji(void)
{
  const char name[] = "FUJINET_DISK_0";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("FUJINET_DISK_0 NOT FOUND");
    cgetc();
  }

  return unit;
}

int8_t sp_find_printer(void)
{
  const char name[] = "PRINTER";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("PRINTER NOT FOUND");
    cgetc();
  }

  return unit;
}

int8_t sp_find_network(void)
{
  const char name[] = "NETWORK";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("NETWORK NOT FOUND");
    cgetc();
  }

  return unit;
}

int8_t sp_find_clock(void)
{
  const char name[] = "FN_CLOCK";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("CLOCK NOT FOUND");
    cgetc();
  }

  return unit;
}

int8_t sp_find_modem(void)
{
  const char name[] = "MODEM";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("MODEM NOT FOUND");
    cgetc();
  }

  return unit;
}

int8_t sp_find_cpm(void)
{
  const char name[] = "CPM";
  int8_t unit = sp_find_device(name, sizeof(name));

  if (unit == 0)
  {
    printf("CPM NOT FOUND");
    cgetc();
  }

  return unit;
}
/**************************/
/**** END OF FIND DEVICES */
/**************************/

void sp_list_devs(void)
{
  int8_t err, num, i, j;

  err = sp_status(0x00, 0x00); // get number of devices
  num = sp_payload[0];
  num++;
  for (i = 1; i < num; i++)
  {
    cprintf("UNIT #%2d NAME: ", i);
    err = sp_status(i, 0x03);
    for (j = 0; j < sp_payload[4]; j++)
      cputc(sp_payload[5 + j]);
    cputs("\r\n");
  }
}

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
  uint8_t s = 0;

  for (s = 7; s > 0; s--)
  {
    uint16_t a = 0xc000 + (s * 0x100);
    if ((PEEK(a + 1) == 0x20) &&
        (PEEK(a + 3) == 0x00) &&
        (PEEK(a + 5) == 0x03) &&
        (PEEK(a + 7) == 0x00))
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
  uint16_t a = 0xC000 + (slot * 0x100);
  uint8_t j = PEEK(a + 0xFF) & 0xFF;

  return a + j + 3;
}

/**
 * Initialize the smartport routines
 * 
 */
uint8_t sp_init(void)
{
  uint8_t slot, f;
  slot = sp_find_slot();
  if (slot)
    sp_dispatch = sp_dispatch_address(slot);
  else
    cprintf("No SmartPort Firmware Found!");

  sp_cmdlist_low = (uint8_t)((uint16_t)&sp_cmdlist & 0x00FF);
  sp_cmdlist_high = (uint8_t)((uint16_t)&sp_cmdlist >> 8) & 0xFF;

  sp_dispatch_addr_low = (uint8_t)((uint16_t)&sp_dispatch & 0x00FF);
  sp_dispatch_addr_high = (uint8_t)((uint16_t)&sp_dispatch >> 8) & 0xFF;

  // sp_list_devs();
  f = sp_find_fuji();
  if (f < 1)
  {
    cprintf("FujiNet Not Found!");
    return 0;
  }

  return 1;
}

/**
 * Determine the number of bytes waiting from the specified device
 * @param dest unit to query
 * @return negative a smartport error has occured. 0 or more: the number of bytes waiting
 */
int16_t sp_bytes_waiting(uint8_t dest)
{
  uint16_t bw;
  int8_t err;

  err = sp_status(dest, 'S');
  if (err)
    return -err;

  bw = sp_payload[0] & 0xFF;
  bw |= sp_payload[1] << 8;

  return bw;
}

/**
 * Returns if the current unit is connected
 * @param dest unit to query
 * @return negative a smartport error has occured. 0: not connected; non-zero connected
 */
int8_t sp_is_connected(uint8_t dest)
{
  uint8_t err;

  err = sp_status(dest, 'S');
  if (err)
    return -err;
  
  return sp_payload[2];
}

/**
 * Accept the incoming connection
 * @param dest unit to query
 * @return 0: no error occured, otherwise the smartport error code.
 */
uint8_t sp_accept(uint8_t dest)
{
  sp_payload[0] = 0;
  sp_payload[1] = 0;
  return sp_control(dest, 'A');
}

#endif /* BUILD_APPLE2 */
