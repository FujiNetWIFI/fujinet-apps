/** 
 * iss-tracker
 *
 * based on Bill Kendrick's Atari version
 *
 * @author thomas cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include "fetch.h"
#include "who.h"

State who(void)
{
  fetch_who();
  return FETCH;
}
