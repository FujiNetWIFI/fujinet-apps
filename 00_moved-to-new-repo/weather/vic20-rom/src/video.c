/**
 * @brief   Wrapper for KERNAL video routines
 * @author  Thom Cherryhomes
 * @email   thom cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#include <vic20.h>
#include <cbm.h>
#include <string.h>
#include "csram.h"

/**
 * @brief set up charset
 */
void video_setup(void)
{
  memcpy(csram,csrom,CSROM_SIZE);
  VIC.addr = 0xFD; // screen memory at 0x1E00, character RAM at 0x1400
}

/**
 * @brief simple screen print line
 * @param s NULL terminated string to print.
 */
void print(const char *s)
{
  while (*s)
    cbm_k_bsout(*s++);
}
