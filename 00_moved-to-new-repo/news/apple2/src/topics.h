/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#ifndef TOPICS_H
#define TOPICS_H

#include "typedefs.h"

typedef enum
  {
    TOPICS_SHOW,
    TOPICS_INPUT,
  } TopicsState;

State topics(void);

#endif /* TOPICS_H */
