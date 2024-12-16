/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#ifndef ARTICLES_H
#define ARTICLES_H

#include "typedefs.h"

typedef enum
  {
    ARTICLES_INIT,
    ARTICLES_LOAD,
    ARTICLES_SHOW,
    ARTICLES_INPUT,
    ARTICLES_UP,
    ARTICLES_DOWN,
    ARTICLES_PAGE_UP,
    ARTICLES_PAGE_DOWN,
    ARTICLES_DONE,
  } ArticlesState;

State articles(void);

#endif /* ARTICLES_H */
