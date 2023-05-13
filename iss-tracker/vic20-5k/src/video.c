/**
 * @brief   Wrapper for KERNAL video routines
 * @author  Thom Cherryhomes
 * @email   thom cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details.
 */

#include <vic20.h>
#include <cbm.h>

/**
 * @brief simple screen print line
 * @param s NULL terminated string to print.
 */
void print(const char *s)
{
  while (*s)
    cbm_k_bsout(*s++);
}
