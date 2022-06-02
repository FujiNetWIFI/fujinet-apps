/**
 *  for Atari Lynx 
 *
 * @brief I/O routines
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 */

#include <6502.h>
#include <lynx.h>
#include <serial.h>
#include <tgi.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char tmp[8];

unsigned char fujinet_init()
{
  struct ser_params params = {
    SER_BAUD_62500,
    SER_BITS_8,
    SER_STOP_1,
    SER_PAR_ODD,
    SER_HS_NONE
  };
  
  ser_install(lynx_comlynx_ser); // This will activate the ComLynx

  CLI();

  return ser_open(&params);
}

unsigned char fujinet_done()
{
  return ser_close();
}

static unsigned char _checksum(unsigned char *b, unsigned short len)
{
  unsigned char checksum = 0x00;
  unsigned short i;
  
  for (i = 0; i < len; i++)
    checksum ^= b[i];
  
  return checksum;
}

/**
 * @brief send packet to FujiNet device
 * @param dev The device # 0x00 - 0x0F
 * @param buf The buffer to send
 * @param len Length of buffer to send
 * @return nack (0xCx) or ack (0x9x)
 */
unsigned char fujinet_send(unsigned char dev, unsigned char *buf, unsigned short len)
{
  char *o = (char *)malloc(len+4);
  unsigned short i=0;
  unsigned char r;
  
  o[0]=0x60 | dev;
  o[2]=len & 0xFF;
  o[1]=len >> 8;
  memcpy(&o[3],buf,len);
  o[len+4]=_checksum((unsigned char *)buf,len);

  // Send data out
  
  for (i=0;i<(len+4);i++)
    ser_put(o[i]);

  free(o);

  // Get rid of data we just sent.

  for (i=0;i<(len+4);i++)
    while (ser_get((char *)&r) == SER_ERR_NO_DATA);

  // Get response

  while (ser_get((char *)&r) == SER_ERR_NO_DATA);
    
  return r;
}

unsigned char fujinet_cts(unsigned char dev)
{
  char b = 0x30 | dev;
  unsigned char r;
  
  ser_put(b);

  // Kill the echo
  while (ser_get((char *)&r) == SER_ERR_NO_DATA);

  // Get response byte.
  while (ser_get((char *)&r) == SER_ERR_NO_DATA);

  return r;
}

unsigned char fujinet_recv(unsigned char dev, unsigned char *buf, unsigned short *len)
{
  char recv_byte = 0x40 | dev;
  char ack_byte  = 0x90 | dev;
  char resp_byte = 0xB0 | dev;
  char nack_byte = 0xC0 | dev;
  char b;
  unsigned short i;
  
  // Return if there's nothing to receive
  ser_put(recv_byte);
  while (ser_get((char *)&b) == SER_ERR_NO_DATA);

  while (ser_get((char *)&b) == SER_ERR_NO_DATA);
    
  if (b == nack_byte)
    return b;
  
  // We got a response, put it in there.
  if (fujinet_cts(dev) == resp_byte)
    {
      // Get first length byte
      while (ser_get((char *)&b) == SER_ERR_NO_DATA);
      *len = b << 8;
      // Get second length byte
      while (ser_get((char *)&b) == SER_ERR_NO_DATA);
      *len |= b & 0xFF;
      
      // Now get the payload
      for (i=0;i<*len;i++)
	{
	  while (ser_get((char *)&b) == SER_ERR_NO_DATA);
	  buf[i]=b;
	}

      // Get the checksum
      while (ser_get((char *)&b) == SER_ERR_NO_DATA);
      ser_put(0x20 | dev); // ACK!
      while (ser_get((char *)&b) == SER_ERR_NO_DATA);
    }
  else
    {
      *len=0;
    }

  return b;
}

