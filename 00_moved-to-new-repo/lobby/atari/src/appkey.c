/**
 * @brief   App Key Functions
 * @author  Oscar Fowler, Thomas Cherryhomes, Eric Carr
 * @license gpl v. 3
 */

#include <string.h>
#include "appkey.h"

static appkeyblock appkey; // The appkey (see appkey.h)


/**
 * @brief Open a key for reading or writing
 * @param open_mode 0 = read, 1 = write
 * @param creator_id Key creator ID: ($0000-$FFFF)
 * @param app_id The App ID ($00-$FF)
 * @param key_id The Key ID ($00-$FF)
 * @return error code
 */
unsigned char sio_openkey(unsigned char open_mode, unsigned int creator_id, unsigned char app_id, unsigned char key_id)
{  
  appkey.open.creator = creator_id;
  appkey.open.app = app_id;
  appkey.open.key = key_id;
  appkey.open.mode = open_mode;
  appkey.open.reserved = 0x00;
  
  OS.dcb.ddevic = SIO_DEVICEID;
  OS.dcb.dunit = SIO_UNITID;
  OS.dcb.dtimlo = SIO_TIMEOUT;
  
  OS.dcb.dcomnd = SIO_CMD_APPKEY_OPEN;
  OS.dcb.dstats = SIO_XFER_WRITE;
  OS.dcb.dbuf = &appkey;
  OS.dcb.dbyt = sizeof(appkey.open);
  OS.dcb.daux = 0;
  
  siov();
  
  return OS.dcb.dstats;
}


unsigned char sio_readkey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char* data)
{
  if (sio_openkey(0,creator_id,app_id,key_id) == 1)
  {
    OS.dcb.ddevic = SIO_DEVICEID;
    OS.dcb.dunit = SIO_UNITID;
    OS.dcb.dtimlo = SIO_TIMEOUT;
    
    OS.dcb.dcomnd = SIO_CMD_APPKEY_READ;
    OS.dcb.dstats = SIO_XFER_READ;
    OS.dcb.dbuf = &appkey;
    OS.dcb.dbyt = sizeof(appkey.read);
    OS.dcb.daux = 0;
    
    siov();
    
    if (OS.dcb.dstats==1)
      strcpy((char *)data,(const char *)appkey.read.value);
  }

  return OS.dcb.dstats;
}

unsigned char sio_writekey(unsigned int creator_id, unsigned char app_id, unsigned char key_id, unsigned char *data)
{
  if (sio_openkey(1,creator_id,app_id,key_id) == 1)
  {
    strcpy((char *)appkey.write.value, (const char *)data);
    OS.dcb.ddevic = SIO_DEVICEID;
    OS.dcb.dunit = SIO_UNITID;
    OS.dcb.dtimlo = SIO_TIMEOUT;
    
    OS.dcb.dcomnd = SIO_CMD_APPKEY_WRITE;
    OS.dcb.dstats = SIO_XFER_WRITE;
    OS.dcb.dbuf = &appkey.write;
    OS.dcb.dbyt = sizeof(appkey.write);
    /* Note that we set DAUX (DAUX1+DAUX2) to the number of valid bytes in the buffer
      even though we're actually sending the full MAX_APPKEY_LEN number of bytes total.
    */
    OS.dcb.daux = strlen((const char *)data);
    
    siov();
  }
  return OS.dcb.dstats;
}

