/**
 * @brief   ---
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#ifndef ARTICLES_H
#define ARTICLES_H

#include "globals.h"

typedef enum _articles_state
{
    ARTICLES_RESET = 0,
    ARTICLES_FETCH,
    ARTICLES_DISPLAY,
    ARTICLES_MENU,
    ARTICLES_UP,
    ARTICLES_DOWN,
    ARTICLES_SELECT,
    ARTICLES_EXIT,
    ARTICLES_NEXT_PAGE,
    ARTICLES_PREV_PAGE,
} ArticlesState;

/** 
 * @brief handle Articles state 
 */
State articles(void);

#endif /* ARTICLES_H */
