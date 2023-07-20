/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Source routines
 */

#include <atari.h>
#include "board.h"
#include "cursor.h"
#include "typedefs.h"
#include "source.h"
#include "msg.h"

/**
 * @brief select source square
 */
void source(void)
{
  msg(msg_source_move);

  while (OS.strig0)
    {
      cursor();
    }

  if (!cursor_get())
    msg(msg_cant_do_that);
    
}
