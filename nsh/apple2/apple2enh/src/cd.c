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

void cd(unsigned char u, const char *s)
{
  // For now, until I plumb out sp.c
  if (u>0)
    return;

  memset(sp_payload,0,sizeof(sp_payload));
  sp_payload[0]=strlen(s)&0xFF;
  sp_payload[1]=strlen(s)>>8;
  strcpy((char *)&sp_payload[2],s);
  sp_control(net[u],',');
}
