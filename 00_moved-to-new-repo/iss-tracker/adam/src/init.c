/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include "init.h"
#include "display.h"
#include "input.h"

State init(void)
{
  display_init();
  input_init();
  return FETCH;
}
