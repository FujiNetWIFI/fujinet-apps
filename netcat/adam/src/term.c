/**
 * @brief FujiLink, a terminal emulator for FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Terminal routines
 */

#include <stdio.h>
#include <conio.h>
#include <video/tms99x8.h>
#include <eos.h>
#include <stdbool.h>
#include "globals.h"

/**
 * @brief AdamNet command completed
 */
#define ACK 0x80

/**
 * @brief Open parameters, see:
 * https://github.com/FujiNetWIFI/fujinet-platformio/wiki/N%3A-SIO-Command-%27O%27---Open
 */
#define READ_WRITE 0x0C
#define NONE 0x00

/**
 * @brief bit 2 of returned status is connected flag
 */
#define CONNECTED 2

/**
 * @brief The receive buffer (1024 bytes)
 */
#define RXBUF_SIZE 1024
unsigned char rxBuf[RXBUF_SIZE];

/**
 * @brief send close command to disconnect
 */
void disconnect(void)
{
  struct _close_cmd
  {
    unsigned char cmd;
    unsigned char padding;
  } closeCmd = {'C',' '};
}

/**
 * @brief check status to see if we are still connected?
 * @return true if connected, false if not.
 */
bool connected(void)
{
  DCB *dcb;

  while (eos_request_device_status(NET_DEV,dcb) < ACK); // grab device status
  return (eos_get_device_status(NET_DEV) & CONNECTED);
}

/**
 * @brief attempt to open connection to current phonebook entry
 */
void connect(void)
{
  unsigned char connect_timer=5;
  
  struct _open_cmd
  {
    unsigned char cmd;
    unsigned char mode;
    unsigned char translation;
    char url[256];
  } openCmd = {'O',READ_WRITE,NONE,"N:TELNET://BBS.RETROCAMPUS.COM:6503/"};

  eos_write_character_device(NET_DEV,openCmd,sizeof(openCmd));

  // Check for connected, once a second, up to 5 tries.
  while (connect_timer--)
    {
      if (connected())
	return true;

      // Nope? wait a moment, and try again.
      sleep(1);
    }

  // Otherwise, we failed. Close and return false.
  disconnect();
  return false;
}

void out(void)
{
  struct _writeCmd
  {
    unsigned char cmd;
    unsigned char payload;
  } writeCMD = {'W',''};

  char c = eos_end_read_keyboard();
  
  if (c>1)
    {
      eos_start_read_keyboard();
      writeCMD.payload=c;
      eos_write_character_device(NET_DEV,writeCMD,sizeof(writeCMD));
    }
}

void in(void)
{
  DCB *dcb = eos_find_dcb(NET_DEV);

  if (eos_read_character_device(NET_DEV,rxBuf,sizeof(rxBuf)) == ACK)
    {
      unsigned short l = dcb->len;

      // Erase Cursor
      putch(0x08);

      // Iterate through received data, taking a break to send any keystrokes.
      for (unsigned short i=0;i<l;i++)
      {
	putch(rxBuf[i]);
      }

      // Place cursor.
      putch('_');
      
    }
}

void term(void)
{
  vdp_set_mode(0);
  vdp_color(VDP_INK_BLACK,VDP_INK_CYAN,VDP_INK_CYAN);

  if (!connect())
    {
      printf("COULD NOT CONNECT.\nPRESS ANY KEY FOR DIALING DIRECTORY.");
      eos_read_keyboard();
      state=DIRECTORY;
      return;
    }

  printf("CONNECTED!\n\n");

  putch('_');
  
  eos_start_read_keyboard();
  
  while(connected())
    {
        out();
        in();
    }

  printf("DISCONNECTED.\nPRESS ANY KEY FOR DIALING DIRECTORY.");
  eos_read_keyboard();
  state=DIRECTORY;
}
