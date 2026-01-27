#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "globals.h"
#include "articles.h"
#include "bar.h"
#include "cocotext.h"
#include "network.h"

#define MAX_ARTICLES_PER_PAGE 19

static ArticlesState articlesState = ARTICLES_RESET;
static char *articles_buffer;
static char line_buffer[81];
static int headline_length = 80;
static int articles_on_this_page = 0;
static char article_cursor_pos = 0;
static char article_cursor_pos_prev = 0;

struct Article {
    int number;
    char id[10];      // use a buffer big enough for a 32-bit long for article ID 
    char title[81];   // fixed size headline buffer
};

struct Article _articles[MAX_ARTICLES_PER_PAGE];

const byte headline_locations[18] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

/**
 * @brief Reset articles state
 */
ArticlesState articles_reset(void)
{
    articles_page = 1;
    article_cursor_pos = 0;
    article_cursor_pos_prev = 0;

	menu_line = 23;
	rows = 21;
	switch (textMode)
	{
	case 40:
	case 42:
		articles_per_page = 18;
		headline_length = 40;
		break;
	case 80:
		articles_per_page = 18;
		headline_length = 80;
		break;
	}

	memset(_articles, 0, MAX_ARTICLES_PER_PAGE * sizeof(struct Article));

    return ARTICLES_FETCH;
}

/**
 * @brief Fetch articles for current page
 */
ArticlesState articles_fetch(void)
{
    strcpy(fetching_buf, "FETCHING ARTICLES, PLEASE WAIT.");

    show_fetching_msg(true);
    
    articles_buffer = fetch_data(true);

    return ARTICLES_DISPLAY;
}

/**
 * @brief Transform and display article headline
 * @param s headline string
 */
char *articles_display_headline(char *s)
{
    memset(line_buffer,0,sizeof(line_buffer));

    strncpy(line_buffer,s,headline_length);

    // Ellipsize if needed.
    if (strlen(line_buffer)==headline_length)
    {
        line_buffer[headline_length-3]=line_buffer[headline_length-2]=line_buffer[headline_length-1]='.';
    }

    return line_buffer;
  
}

char *centered_line(const char *text, int width)
{
    memset(line_buffer, 0, sizeof(line_buffer));

    // Fill with spaces
    memset(line_buffer, ' ', width);
    line_buffer[width] = '\0';

    // Clamp text length to width
    int text_len = strlen(text);
    if (text_len > width)
        text_len = width;

    // Compute centered start position
    int text_start = (width - text_len) / 2;

    // Copy text into centered position
    memcpy(line_buffer + text_start, text, text_len);

    return line_buffer;
}

/**
 * @brief _articles[ the articles response
 * @param input API response string
 * @param articles caller-provided buffer of struct Article[]
 * @param capacity max number of articles that fit in the buffer
 * @return Number of artticles on the page if success, -1 on error
 */
int parse_articles_response(char *input, struct Article *articles, int capacity)
{
    int count = 0;
    char *p = input;

    while (*p && count < capacity)
    {
        char *line = p;
        char *nl = strchr(p, '\n');
        char *comma;
        int i;

        /* terminate this line */
        if (nl) { *nl = 0; p = nl + 1; }
        else    { p += strlen(p); }

        comma = strchr(line, ',');
        if (!comma) continue;

        *comma++ = 0;           /* line now points to title */
        if (*comma == ' ') comma++;   /* skip single space after comma */

        /* copy title */
        i = 0;
        while (i < 80 && line[i]) { articles[count].title[i] = line[i]; i++; }
        articles[count].title[i] = 0;

        /* copy id */
        i = 0;
        while (i < 11 && comma[i]) { articles[count].id[i] = comma[i]; i++; }
        articles[count].id[i] = 0;

        count++;
    }

    return count;
}


/**
 * @brief Display fetched articles
 */
ArticlesState articles_display(void)
{
	int i = 0;
	int w;
	byte y;

	hd_bar(0, centered_line(search_term, textMode), true);

	articles_on_this_page = parse_articles_response(articles_buffer, _articles, MAX_ARTICLES_PER_PAGE);

		for (int i = 0; i < articles_on_this_page; i++)
		{
            w = textMode;
            y = headline_locations[i];

			gotoxy(0, y);
			printf("%-*s", w, articles_display_headline(_articles[i].title));
		}

		multiline_hd_bar(headline_locations[article_cursor_pos], 1, _articles[article_cursor_pos].title, true);

	return ARTICLES_MENU;
}

/**
 * @brief Update articles menu bar
 */
void articles_bar(void)
{
	if (article_cursor_pos_prev != article_cursor_pos)
	{
		multiline_hd_bar(headline_locations[article_cursor_pos_prev], 1, _articles[article_cursor_pos_prev].title, false);
		multiline_hd_bar(headline_locations[article_cursor_pos], 1, _articles[article_cursor_pos].title, true);
	}
	article_cursor_pos_prev = article_cursor_pos;
}

/**
 * @brief Handle menu for current article page
 */
ArticlesState articles_menu(void)
{
    print_reverse(0, menu_line-1,   "up/down CHOOSE break SEARCH enter VIEW", true);

    if (hirestxt_mode)
    {
        print_reverse(0, menu_line, "w SET SCREEN WIDTH c TOGGLE COLOR", true);
    }
    else
    {
        print_reverse(0, menu_line, "w SET SCREEN WIDTH", true);
    }

    articles_bar();

    gotoxy(textMode-1, menu_line);

    switch (waitkey(false))
    {
    case BREAK:
        return ARTICLES_EXIT;
    case ARROW_UP:
        return ARTICLES_UP;
    case ARROW_DOWN:
        return ARTICLES_DOWN;
    case ENTER:
        return ARTICLES_SELECT;
    case 'C':
    case 'c':
        switch_colorset();
        return ARTICLES_MENU;
    case 'W':
    case 'w':
        if (isCoCo3)
        {
            select_screen_width();
            return ARTICLES_RESET;
        }
        break;
    }

    return ARTICLES_MENU;
}

/**
 * @brief handle moving up, for current articles page
 */
ArticlesState articles_up(void)
{
    if (article_cursor_pos >  0)
    {
        articles_bar();
        article_cursor_pos--;
    }

    return ARTICLES_MENU;
}

/**
 * @brief handle moving down, for current articles page
 */
ArticlesState articles_down(void)
{
    if (article_cursor_pos<articles_on_this_page-1)
    {
        articles_bar();
        article_cursor_pos++;
    }

    return ARTICLES_MENU;
}


/** 
 * @brief handle Articles state 
 */
State articles(void)
{
    articlesState=ARTICLES_RESET;
    
    while (state == ARTICLES)
    {
        switch(articlesState)
        {
        case ARTICLES_RESET:
            articlesState = articles_reset();
            break;
        case ARTICLES_FETCH:
            articlesState = articles_fetch();
            break;
        case ARTICLES_DISPLAY:
            articlesState = articles_display();
            break;
        case ARTICLES_MENU:
            articlesState = articles_menu();
            break;
        case ARTICLES_UP:
            articlesState = articles_up();
            break;
        case ARTICLES_DOWN:
            articlesState = articles_down();
            break;
        case ARTICLES_SELECT:
            article_id = _articles[article_cursor_pos].id;
            return ARTICLE;
        case ARTICLES_EXIT:
            return SEARCH;
        }
    }

    return SEARCH;
}
