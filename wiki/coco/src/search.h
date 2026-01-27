
#ifndef SEARCH_H
#define SEARCH_H

#include "globals.h"

typedef enum _search_state
{
    SEARCH_RESET = 0,
    SEARCH_GET_SEARCH_TERM,
    SEARCH_TERM_SELECTED,
    SEARCH_EXIT,
} SearchState;

extern SearchState searchState;

/**
 * @brief Handle Topics menu
 */
State search(void);

#endif /* SEARCH_H */
