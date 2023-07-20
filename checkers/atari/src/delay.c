/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Source routines
 */

#include <atari.h>

/**
 * @brief wait d vblanks
 * @param d # of vblanks to wait (0-255)
 */
void delay(unsigned char d)
{
  unsigned char i;
  
  for (i=0;i<d;i++)
    waitvsync();
}
