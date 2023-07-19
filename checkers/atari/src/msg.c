/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Message line routines
 */

#include <stdlib.h>
#include <string.h>
#include "constants.h"

/**
 * @brief convert ASCII to charset code
 */
static char msg_char(char c)
{
  char offset=0;
  if (c < 32)
    {
      offset=64;
    }
  else if (c<96)
    {
      offset=-32;
    }

  return c += offset;
}

/**
 * @brief place s onto message bar
 */
void msg(char *s)
{
  unsigned char i;
  
  bzero(MSG,20);

  for (i=0;i<strlen(s);i++)
    MSG[i]=msg_char(s[i]);
}
