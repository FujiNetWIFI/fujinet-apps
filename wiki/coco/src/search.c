/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Topics menu
 */

#include <cmoc.h>
#include <coco.h>
#include "globals.h"
#include "search.h"
#include "bar.h"
#include "cocotext.h"

char *centered_line(const char *text, int width);

#define MENU_Y_16 3
#define MENU_Y_24 8

SearchState searchState = SEARCH_RESET;

/**
 * @brief Reset topics menu 
 */
SearchState search_reset(void)
{
	clear_screen(1);
	gotoxy(0, 0);
	switch (textMode)
	{
	case 40:
		hd_bar(0, "     FujiNet Wikipedia Article Viewer", true);
		gotoxy(0, menu_line);
		printf("%-9s%s", " ", "Press <BREAK> to exit.");
		break;
	case 42:
		hd_bar(0, "      FujiNet Wikipedia Article Viewer", true);
		gotoxy(0, menu_line);
		printf("%-9s%s", " ", "Press <BREAK> to exit.");
		break;
	case 80:
		hd_bar(0, "                        FujiNet Wikipedia Article Viewer", true);
		gotoxy(0, menu_line);
		printf("%-29s%s", " ", "Press <BREAK> to exit.");
		break;
	}

	return SEARCH_GET_SEARCH_TERM;
}

SearchState search_get_search_term(void)
{
	SearchState get_search_state = SEARCH_GET_SEARCH_TERM;
    
    while (get_search_state == SEARCH_GET_SEARCH_TERM)
	{

		gotoxy(0, 2);
		memset(search_term, 0, 115);
		printf("Enter Search Term:\n");
		if (!get_line(search_term, 115))
        {
			get_search_state = SEARCH_EXIT;
		}
		else if (strlen(search_term) > 0)
		{
			get_search_state = SEARCH_TERM_SELECTED;
		}
		else
		{
			get_search_state = SEARCH_GET_SEARCH_TERM;
		}
	}

    return get_search_state;
}

State search(void)
{
    searchState=SEARCH_RESET;
    
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
