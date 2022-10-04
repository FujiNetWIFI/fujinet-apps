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

TopicsState topics_show(void)
{
  return TOPICS_SHOW;
}

TopicsState topics_up(void)
{
  return TOPICS_SHOW;
}

TopicsState topics_down(void)
{
  return TOPICS_SHOW;
}

State topics(void)
{
  switch (subState)
    {
    case TOPICS_SHOW:
      subState=topics_show();
      break;
    case TOPICS_UP:
      subState=topics_up();
      break;
    case TOPICS_DOWN:
      subState=topics_down();
      break;
    case TOPICS_DONE:
      return DONE;
    }

  return TOPICS;
}
