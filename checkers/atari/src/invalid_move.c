/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Invalid Move routines
 */

#include "typedefs.h"
#include "msg.h"

/**
 * @brief frame delay/counters
 */
extern unsigned char frame_counter, frame_delay;

/**
 * @brief state variables
 */
extern State state, return_state;

/**
 * @brief called when in INVALID MOVE state
 */
void invalid_move(void)
{
  if (!frame_delay)
    msg(msg_cant_do_that);

  // pre-increment count-down and ensure it wraps at 64.
  frame_delay--;
  frame_delay &= 0x3F;

  if (!frame_delay)
    state=return_state;
}
