#ifndef ARTICLE_H
#define ARTICLE_H

#include "globals.h"

typedef enum _article_state
{
    ARTICLE_RESET = 0,
    ARTICLE_FETCH,
    ARTICLE_DISPLAY,
    ARTICLE_MENU,
    ARTICLE_NEXT_PAGE,
    ARTICLE_PREV_PAGE,
    ARTICLE_EXIT
} ArticleState;

State article(void);

#endif /* ARTICLE_H */
