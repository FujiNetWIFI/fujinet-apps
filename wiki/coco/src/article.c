/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Article display
 */

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "globals.h"
#include "article.h"
#include "bar.h"
#include "cocotext.h"
#include "network.h"

/**
 * @brief Article Page buffer 
 */
static char *article_page_buffer;

/**
 * @brief Article metadata
 */
static char *title = NULL;
static char *page = NULL;
static char *pageData = NULL;
static int page_max = 0;

/**
 * @brief current state for article view
 */
static ArticleState articleState;

static char menu_line_buffer[81];

char *articles_display_headline(char *s);

/**
 * @brief Initial entrypoint, reset local state
 */
ArticleState article_reset(void)
{
	article_page = 1;
    if (textMode == 80)
    {
        rows = 19;
    }
    else
    {
        rows = 18;
    }

	return ARTICLE_FETCH;
}

void parse_article_response(char *input)
{
    char *p;
    char *slash;

    if (!input)
        return;

    /* First line = title */
    title = input;

    /* Find first newline */
    p = strchr(input, '\n');
    if (!p)
        return;

    *p = '\0';          /* terminate title */
    p++;

    /* Second line = page */
    page = p;
    /* Move past "Pg:"*/
    page += 3;

    /* Find second newline */
    p = strchr(p, '\n');
    if (!p)
        return;

    *p = '\0';          /* terminate page line */
    p++;

    /* Rest = pageData */
    pageData = p;

    /* Extract page_max from "Pg:X/Y" */

    slash = strchr(page, '/');
    if (slash)
        page_max = atoi(slash + 1);
    else
        page_max = 0;
}

/**
 * @brief Fetch current article page
 */
ArticleState article_fetch(void)
{
    strcpy(fetching_buf, "FETCHING PAGE, PLEASE WAIT.");

    show_fetching_msg(true);

    article_page_buffer = fetch_data(false);
    
    parse_article_response(article_page_buffer);
 
    return ARTICLE_DISPLAY;
}

/**
 * @brief Display current article page
 */
ArticleState article_display(void)
{
	clear_screen(1);

    multiline_hd_bar(0, 1, articles_display_headline(title), true);


	printf("\n\n");
	printf("%s", pageData);

	return ARTICLE_MENU;
}

/**
 * @brief Handle article menu
 */
ArticleState article_menu(void)
{
	memset(menu_line_buffer, 0, sizeof(menu_line_buffer));

	sprintf(menu_line_buffer, "%-7s %s", page, "up/down PAGE break ARTICLES");
	print_reverse(0, menu_line-1, menu_line_buffer, true);
	if (hirestxt_mode)
	{
        print_reverse(0, menu_line, "w SET SCREEN WIDTH c TOGGLE COLOR", true);
    }
    else
    {
        print_reverse(0, menu_line, "w SET SCREEN WIDTH", true);
    }
    
	gotoxy(textMode - 1, menu_line);

	switch (waitkey(0))
	{
	case BREAK:
		return ARTICLE_EXIT;
	case ARROW_UP:
		return ARTICLE_PREV_PAGE;
	case ARROW_DOWN:
		return ARTICLE_NEXT_PAGE;
	case 'C':
	case 'c':
		switch_colorset();
		return ARTICLE_MENU;
	case 'W':
	case 'w':
		if (isCoCo3)
		{
			select_screen_width();
			return ARTICLE_RESET;
		}
		break;
	}

	return ARTICLE_MENU;
}

/**
 * @brief Go to next page 
 */
ArticleState article_next_page(void)
{
    article_page++;

    if (article_page > page_max)
        return ARTICLE_MENU;
    
    return ARTICLE_FETCH;
}

/**
 * @brief Go to previous page
 */
ArticleState article_prev_page(void)
{
    if (article_page == 1)
        return ARTICLE_MENU;

    article_page--;
    
    return ARTICLE_FETCH;
}

/**
 * @brief Handle article state
 */
State article(void)
{
    articleState=ARTICLE_RESET;

    while (state == ARTICLE)
    {
        switch(articleState)
        {
        case ARTICLE_RESET:
            articleState = article_reset();
            break;
        case ARTICLE_FETCH:
            articleState = article_fetch();
            break;
        case ARTICLE_DISPLAY:
            articleState = article_display();
            break;
        case ARTICLE_MENU:
            articleState = article_menu();
            break;
        case ARTICLE_NEXT_PAGE:
            articleState = article_next_page();
            break;
        case ARTICLE_PREV_PAGE:
            articleState = article_prev_page();
            break;
        case ARTICLE_EXIT:
            return ARTICLES;            
        }
    }

    return ARTICLES;
}
