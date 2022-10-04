/**
 * #FUJINET News
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#ifndef ARTICLE_H
#define ARTICLE_H

#include "typedefs.h"

typedef enum
  {
    ARTICLE_SHOW,
    ARTICLE_PAGE_UP,
    ARTICLE_PAGE_DOWN,
    ARTICLE_INFO,
    ARTICLE_DONE,
  } ArticleState;

State article(void);

#endif /* ARTICLE_H */
