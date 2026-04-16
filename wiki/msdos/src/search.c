#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "search.h"
#include "screen.h"

SearchState searchState = SEARCH_RESET;

static SearchState search_reset(void)
{
    screen_clear();
    hd_bar(0, "FujiNet Wikipedia Article Viewer", ATTRIBUTE_HEADER);
    screen_fill_line(23, ATTRIBUTE_STATUS);
    screen_puts_center(23, ATTRIBUTE_STATUS, "Press <ESC> to exit.");

    return SEARCH_GET_SEARCH_TERM;
}

static SearchState search_get_search_term(void)
{
    SearchState get_search_state = SEARCH_GET_SEARCH_TERM;

    while (get_search_state == SEARCH_GET_SEARCH_TERM)
    {
        memset(search_term, 0, 115);
        screen_puts(0, 2, ATTRIBUTE_NORMAL, "Enter Search Term:");
        gotoxy(0, 3);
        if (!get_line(search_term, 115))
        {
            get_search_state = SEARCH_EXIT;
        }
        else if (strlen(search_term) > 0)
        {
            get_search_state = SEARCH_TERM_SELECTED;
        }
    }

    return get_search_state;
}

State search(void)
{
    searchState = SEARCH_RESET;

    while (state == SEARCH)
    {
        switch (searchState)
        {
        case SEARCH_RESET:
            searchState = search_reset();
            break;
        case SEARCH_GET_SEARCH_TERM:
            searchState = search_get_search_term();
            break;
        case SEARCH_TERM_SELECTED:
            return ARTICLES;
        case SEARCH_EXIT:
            return BYE;
        }
    }

    return BYE;
}
