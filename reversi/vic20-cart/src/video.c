/**
 * @brief   Wrapper for KERNAL video routines
 * @author  Thom Cherryhomes
 * @email   thom cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#include <vic20.h>
#include <cbm.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
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
 * @brief read color ram reliably (mask of unused upper nibble)
 * @param color ram offset
 * @return color at offset
 */
unsigned char video_ram_color(unsigned short o)
{
  return COLOR_RAM[o] & 0x0F;
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

/**
 * @brief simple line input
 * @param s pointer to string
 */
void input(char *s)
{
  unsigned char c,i;
  
  c=i=0;

  while (true)
    {
      cursor(1);
      c = cgetc();

      if ((c == 0x14) && i) // DEL is DC4 ?!
	i--;
      else if (c==0x0D)
	{
	  cbm_k_bsout(0x0A);
	  break;
	}
      else
	s[i++] = c;

      cbm_k_bsout(c);
    }
}
