/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <apple2.h>
#include <conio.h>
#include "sp.h"
#include "topics.h"
#include "typedefs.h"

static TopicsState subState=TOPICS_SHOW;

State topics(void)
{
  switch (subState)
    {
    case TOPICS_SHOW:
      break;
    case TOPICS_UP:
      break;
    case TOPICS_DOWN:
      break;
    case TOPICS_DONE:
      return DONE;
    }

  return TOPICS;
}
