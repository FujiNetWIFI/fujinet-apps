/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Article Display
 */

#ifndef ARTICLE_H
#define ARTICLE_H

#include "globals.h"

/**
 * @brief currently selected article
 */
extern long selectedArticle;

typedef enum _article_state
    {
        ARTICLE_RESET = 0,
        ARTICLE_FETCH,
        ARTICLE_DISPLAY,
        ARTICLE_MENU,
        ARTICLE_NEXT_PAGE,
        ARTICLE_PREV_PAGE,
        ARTICLE_INFO,
        ARTICLE_EXIT
    } ArticleState;

/**
 * @brief handle article state*/
State article(void);

#endif /* ARTICLE_H */
