/**
 * @brief   Network I/O Functions for #FujiNet
 * @verbose Using the SmartPort
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 *
 * @license gpl v. 3
 */

#include <string.h>
#include "nio.h"
#include "die.h"
#include "sp.h" /* Requires SmartPort functions */

/**
 * @brief Open Network connection
 * @param net   Network device ID
 * @param url   The URL to open (e.g. N:TCP://1.2.3.4:6502/)
 * @param mode  The opening mode (e.g. 12 = read/write)
 * @param trans The translation mode (usually 0)
 * @return err  SmartPort error code, or 0 if none.
 */
char nopen(unsigned char net, char *url, unsigned char mode, unsigned char trans)
{  
  /* Construct payload for OPEN control */
  sp_payload[0]=(strlen(url) & 0xFF) + 2; /* length is + 2 to account for mode and trans bytes. */
  sp_payload[1]=(strlen(url) >> 8);
  sp_payload[2]=mode;
  sp_payload[3]=trans;
  strcpy((char *)&sp_payload[4],url);

  /* Do the open */
  return sp_control(net,'O');
}

/**
 * @brief  Close Network Connection
 * @return SmartPort error code, or 0 if none.
 */
char nclose(unsigned char net)
{
  sp_payload[0]=sp_payload[1]=0; /* No payload */

  return sp_control(net,'C'); /* Do the close */
}

/**
 * @brief  Get Network Status
 * @param  net The smartport network device ID to request status
 * @return # of bytes waiting, connected?, and last known error in ptrs, 
 *         smartport error in return or 0 if ok
 */
char nstatus(unsigned char net, unsigned short *bw, bool *connected, unsigned char *nerr)
{
  char err = sp_status(net,'S');

  if (err)
    return err;

  if (bw != NULL)
    {
      *bw  = sp_payload[0];
      *bw |= sp_payload[1] << 8;
    }

  if (connected != NULL)
    *connected = sp_payload[2];

  if (nerr != NULL)
    *nerr = sp_payload[3];

  return 0; /* Everything ok */
}

/**
 * @brief  Read requested # of bytes into buf
 * @param  net The smartport Network device ID 
 * @param  buf The target buffer, assumed to be large enough
 * @param  len The # of bytes to read into buffer, assumed < buf len
 * @return smartport error, or 0 if ok.
 */
char nread(unsigned char net, void *buf, unsigned short len)
{
  char err = sp_read(net,len);

  if (err)
    return err;

  /* Read successful, copy data into target buffer */
  memcpy(buf,&sp_payload[0],len);
  
  return 0; /* Everything ok */
}

/**
 * @brief  send requested # of bytes from buf
 * @param  net The smartport Network device ID 
 * @param  buf The source buffer, assumed to be large enough
 * @param  len The # of bytes to send from buffer, assumed < buf len
 * @return smartport error, or 0 if ok.
 */
char nwrite(unsigned char net, void *buf, unsigned short len)
{
  /* Copy source buffer into payload */
  memcpy(&sp_payload[0],buf,len);
  
  return sp_write(net,len);
}

/**
 * @brief  Send control command to network device
 * @param  net the smartport network device ID
 * @param  cmd The command byte to do.
 * @param  The command buffer to send
 * @param  The command buffer length
 * @return smartport error, or 0 if ok.
 */
char ncontrol(unsigned char net, unsigned char cmd, void *buf, unsigned short len)
{
  sp_payload[0]=sp_payload[1]=0;
  
  if (len>0)
    {
      sp_payload[0]=len&0xFF;
      sp_payload[1]|=len<<8;
      memcpy(&sp_payload[2],buf,len);
    }
  
  return sp_control(net,cmd);
}

/**
 * @brief Set channel mode for a network device
 * @param net  The network device
 * @param mode the new channel mode
 * @return smartport error, or 0 if ok
 */
char nchannelmode(unsigned char net, unsigned char mode)
{
  sp_payload[0]=1;
  sp_payload[1]=0;
  sp_payload[2]=mode;

  return sp_control(net,0xFC);
}

/**
 * @brief Send open channel to JSON parser
 * @param net  The network device
 * @return smartport error, or 0 if ok.
 */
char njsonparse(unsigned char net)
{
  char err = nchannelmode(net,CHANNEL_MODE_JSON);

  if (err)
    return err;
  
  sp_payload[0]=sp_payload[1]=0;
  
  return sp_control(net,'P');
}

/**
 * @brief JSON-Pointer for JSON path
 * @param net  The Network smartport device ID
 * @param path The JSON-Pointer path a la RFC 6901
 */
char njsonquery(unsigned char net, char *path)
{
  sp_payload[0]=strlen(path)&0xFF;
  sp_payload[1]|=strlen(path)<<8;
  return sp_control(net,'Q');
}
