/**
 * @brief Bitmap viewer over network using #FujiNet
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 * @verbose Main program
 */

#include <eos.h>
#include <msx.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NET_DEV 0x09
#define READ_WRITE "\x0c"
#define NO_TRANSLATION "\x00"
#define PICTURE_BUFFER_SIZE 12288  /* GRAPHICS II Pixels (6144) + Color data (6144) */
#define CONNECTION_WAITING 0x02    /* STATUS byte when connection is waiting */
#define NETWORK_RESPONSE_SIZE 1024

const char *listen_url="N:TCP://:6502/";

unsigned char picture_buffer[PICTURE_BUFFER_SIZE];

unsigned char open_listening_connection(void)
{
  unsigned char open_cmd[259]; // 256 + 3 command bytes

  open_cmd[0]='O';
  open_cmd[1]=READ_WRITE;
  open_cmd[2]=NO_TRANSLATION;

  strncpy((char *)&open_cmd[3],listen_url,256);

  return eos_write_character_device(NET_DEV,open_cmd,sizeof(open_cmd));
}

void wait_for_connection(void)
{
  DCB *dcb = NULL;
  unsigned char status=0;

  cprintf("WAITING.\n");
  
  while (status!=CONNECTION_WAITING)
    {
      eos_request_device_status(NET_DEV,dcb);
      status = eos_get_device_status(NET_DEV);
    }

}

void accept_waiting_connection(void)
{
  unsigned char accept_cmd[3]={'A',READ_WRITE,NO_TRANSLATION};

  eos_write_character_device(NET_DEV,&accept_cmd,sizeof(accept_cmd));

  cprintf("CONNECTED.\n");
}

void read_incoming_picture(void)
{  
  cprintf("READING INCOMING PICTURE\n");
  
  for (unsigned char i=0;i<12;i++)
    {
      eos_read_character_device(NET_DEV,&picture_buffer[i*NETWORK_RESPONSE_SIZE],NETWORK_RESPONSE_SIZE);
      cprintf("%5u BYTES\n",i*NETWORK_RESPONSE_SIZE);
    }
}

void display_incoming_picture(void)
{
  clrscr();
  msx_vwrite(&picture_buffer[0],0x0000,6144);      // Write the pixels to the VDP
  msx_vwrite(&picture_buffer[6144],0x2000,6144);   // Write the color data to the VDP
}

void close_connection(void)
{
  unsigned char close_cmd[3]={'c',0x00,0x00}; // lowercase c means CLOSE ACCEPTED CONNECTION
  eos_write_character_device(NET_DEV,&close_cmd,sizeof(close_cmd));
}

void main(void)
{
  open_listening_connection();

  while(true)
    {
      wait_for_connection();
      accept_waiting_connection();
      read_incoming_picture();
      display_incoming_picture();
      close_connection();
    }
    
}
