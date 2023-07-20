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

const char *msg_source_move=" SELECT MOVE FROM:";
const char *msg_cant_do_that=" CAN'T DO THAT.";

/**
 * @brief convert ASCII to charset code
 * @param c Character to translate to charset code
 * @return internal code for character.
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
 * @param s String to display in message bar (0-20 chars)
 */
void msg(const char *s)
{
  unsigned char i;
  
  bzero(MSG,20);

  for (i=0;i<strlen(s);i++)
    MSG[i]=msg_char(s[i]);
}
