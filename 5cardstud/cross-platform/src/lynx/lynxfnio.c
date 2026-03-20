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
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "lynxfnio.h"


#define RECV_TIMEOUT  10   // timeout for receive loop, in seconds

// Global variables
unsigned char _ck;			// checksum byte
char _r;               		// response/data from FN
unsigned char _fn_error;	// error status


unsigned char fnio_init(void)
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


unsigned char fnio_done(void)
{
  return ser_close();
}


/**
 * @brief calculates the checksum on the packet and stores in global var
 * @param *b pointer to buffer
 * @param len length of buffer
 */
void _checksum(char *b, unsigned short len)
{
  register unsigned short i;


  // initialize checksum to zero
  _ck = 0;

  for (i = 0; i < len; ++i)
    _ck ^= b[i];

  return;
}


/**
 * @brief loop to get data that is used many times
 * @param b The buffer to receive into
 */
unsigned char _serial_get_loop(void)
{
  clock_t start, now;

  start = clock();
  while (ser_get(&_r) == SER_ERR_NO_DATA) {
    now = clock();
    if (((now - start) / CLOCKS_PER_SEC) > RECV_TIMEOUT) {
      //_r = FUJICMD_NAK;
      _fn_error = FNIO_ERR_TIMEOUT;
      return(0);
    }
  }

  return(1);
}


void fnio_flush_recv(void)
{
  while (ser_get(&_r) != SER_ERR_NO_DATA);
}


/**
 * @brief send packet to FujiNet device
 * @param dev The device # 0x00 - 0x0F
 * @param buf The buffer to send
 * @param len Length of buffer to send
 * @return 1 on ACK, 0 on NAK
 */
unsigned char fnio_send_buf(unsigned char dev, char *buf, unsigned short len)
{
  register unsigned short i;
  unsigned char ret;


  // reset error status
  _fn_error = FNIO_ERR_NONE;

  // Calculate checksum on buffer
  _checksum(buf, len);

  // Send the Device and Length
  ser_put(dev);
  ser_put(len >> 8);
  ser_put(len & 0xFF);
  ser_get(&_r);           // get rid of reflected data
  ser_get(&_r);
  ser_get(&_r);

  // send the payload
  for(i=0; i<len; ++i) {
    //while (ser_put(buf[i]) != SER_ERR_OVERFLOW);        // handle if we overflowed the TX buffer
    ser_put(buf[i]);
    ser_get(&_r);         // get rid of reflected data we just sent
  }

  // send the checksum
  ser_put(_ck);
  ser_get(&_r);           // get rid of reflected data

  // Get response
  ret = _serial_get_loop();
  if (!ret)
	  return(0);

  // r contains our response, ACK or NACK
  if (_r == FUJICMD_ACK)
    return(1);
  else {
	_fn_error = FNIO_ERR_SEND_CHK;
    return(0);
	}
}


unsigned char fnio_recv_buf(char *buf, unsigned short *len)
{
  register unsigned short i;
  unsigned char t;


  // reset error status
  _fn_error = FNIO_ERR_NONE;

  // Get first length byte
  t = _serial_get_loop();
  if (!t)
	return(0);
  *len = _r << 8;

  // Get second length byte
  t = _serial_get_loop();
  if (!t)
  	return(0);
  *len |= _r & 0xFF;

  if (*len > LEN_MAX)       // no more than LEN_MAX bytes
	  *len = LEN_MAX;

  // Now get the payload
  for (i=0; i<*len; ++i) {
	  t = _serial_get_loop();
	  if (!t)
	  	return(0);
   	buf[i] = _r;
  }

  // Get the checksum
  t = _serial_get_loop();
  if (!t)
  	return(0);                            // timeout

  // checksum matches?
  _checksum(buf, *len);
  if (_r == _ck) {
	  ser_put(FUJICMD_ACK);								  // ACK
    ser_get((char *) &_r);                // get reflected data
    return(1);                            // succes, checksum matches
  }
  else {
	  _fn_error = FNIO_ERR_RECV_CHK;
	  ser_put(FUJICMD_NAK); 								// NACK, checksum bad
    ser_get((char *) &_r);                // get reflected data
	  *len = 0;											        // return zero length
    return(0);                            // checksum bad
  }
}


// receive an ACK or NAK
// returns 1 on ACK received or 0 on NAK
// Call after commands that don't send back any data
unsigned char fnio_recv_ack(void)
{
	unsigned char t;

  // reset error status
  _fn_error = FNIO_ERR_NONE;

  t = _serial_get_loop();
  if (!t)
  	return(0);

  if (_r == FUJICMD_ACK)
    return(1);
  else {
	  _fn_error = FNIO_ERR_GENERAL;
    fnio_flush_recv();
    return(0);
	}
}


unsigned char fnio_error()
{
  return(_fn_error);
}