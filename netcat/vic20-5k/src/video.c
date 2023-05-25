/**
 * @brief   Wrapper for KERNAL video routines
 * @author  Thom Cherryhomes
 * @email   thom cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include <stdbool.h>

static unsigned char c,i;

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
  c=i=0;

  while (true)
    {
      cursor(1);
      c = cgetc();

      if ((c == 0x08) && i)
	i--;
      else if (c==0x0D)
	break;
      else
	s[i++] = c;

      cbm_k_bsout(c);
    }
}
