/**
 * Network Utilities
 * 
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * 
 * @brief   Returns the current prefix for a network unit.
 */

#include <string.h>
#include "sp.h"

extern unsigned char net[4];

const char *pwd_notset="<PREFIX NOT SET>";

const char *pwd(unsigned char u)
{
  // For now, until I plumb out sp.c
  if (u>0)
    return pwd_notset;

  memset(sp_payload,0,sizeof(sp_payload));
  if (sp_status(net[0],'0') == SP_ERR_NOERROR)
    if (sp_payload[0]==0x00)
      return pwd_notset;
    else
      return (const char *)sp_payload;
  
  return NULL;
}
