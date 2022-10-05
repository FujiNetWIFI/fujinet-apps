/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#include <stdlib.h>
#include "dcb.h"

#define NET_DEV 0x09

/**
 * @brief Find FujiNet DCB
 * @return Pointer to PCB
 */
DCB* find_dcb(void)
{
  char num_dcbs =  (*(unsigned char*) 0xFEC3);
  DCB *current_dcb = (*(DCB *)0xFEC4);
  char i;

  for (i=0;i<num_dcbs;i++)
    {
      if ((current_dcb[i].dev & 0x0F) == NET_DEV)
	return current_dcb[i];
    }

  // Device not found, return NULL.
  return NULL;
}
