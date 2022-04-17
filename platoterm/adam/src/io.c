/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.h - Input/output functions (serial/ethernet)
 */

#include <stdbool.h>
#include <eos.h>
#include <string.h>
#include "io.h"
#include "protocol.h"

#define NET 0x09

static bool connected=false;

static unsigned char inb;
static unsigned char response[1024];
DCB *dcb;

/**
 * io_init() - Set-up the I/O
 */
void io_init(void)
{
  struct _oc
  {
    unsigned char cmd;
    unsigned char mode;
    unsigned char translation;
    unsigned char url[30];
  } OC;

  // Go ahead and close the channel just in case.
  io_done();
  
  dcb = eos_find_dcb(NET);

  OC.cmd='O';
  OC.mode=12; /* READ/WRITE */
  OC.translation=0; /* NONE */
  strcpy(OC.url,"N:TCP://IRATA.ONLINE:8005/");

  /* PERFORM OPEN COMMAND */
  eos_write_character_device(NET,(unsigned char *)OC,sizeof(OC));

  connected=true;
}

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(unsigned char b)
{
  struct _wc
  {
    unsigned char cmd;
    unsigned char byte;
  } WC;

  WC.cmd = 'W';
  WC.byte = b;

  if (connected == true)
    eos_write_character_device(NET,(unsigned char *)WC,sizeof(WC));
}

/**
 * io_main() - The IO main loop
 * @return false = we've disconnected.
 */
bool io_main(void)
{
  unsigned char r=eos_read_character_device(NET,response,sizeof(response));

  if (r==0x80)
      ShowPLATO(response,dcb->len);

  return true;
}

/**
 * io_done() - Called to close I/O
 */
void io_done(void)
{
  eos_write_character_device(NET,"C",1);
}
