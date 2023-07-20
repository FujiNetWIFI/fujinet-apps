/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Destination routines
 */

#include <atari.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "destination.h"
#include "source.h"
#include "msg.h"

/**
 * @brief select destination square
 */
void destination(void)
{
  msg(msg_destination_move);

  while (OS.strig0)
    {
      source_blink();
      cursor();
    }
}
