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
} ArticlesState;

State articles(void);

#endif /* ARTICLES_H */
