#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "articles.h"
#include "bar.h"
#include "screen.h"
#include "network.h"

#define MAX_ARTICLES_PER_PAGE 19

static ArticlesState articlesState = ARTICLES_RESET;
static char *articles_buffer;
static char line_buffer[81];
static int articles_on_this_page = 0;
static char article_cursor_pos = 0;
static char article_cursor_pos_prev = 0;

struct Article {
    int number;
    char id[10];
    char title[81];
};

static struct Article _articles[MAX_ARTICLES_PER_PAGE];

static const unsigned char headline_locations[18] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

static char *articles_display_headline(char *s)
{
    int headline_length = screen_cols;

    memset(line_buffer, 0, sizeof(line_buffer));
    strncpy(line_buffer, s, headline_length);

    if ((int)strlen(line_buffer) == headline_length)
    {
        line_buffer[headline_length - 3] = '.';
        line_buffer[headline_length - 2] = '.';
        line_buffer[headline_length - 1] = '.';
    }

    return line_buffer;
}

static char *centered_line(const char *text, int width)
{
    int text_len, text_start;

    memset(line_buffer, ' ', width);
    line_buffer[width] = '\0';

    text_len = strlen(text);
    if (text_len > width)
        text_len = width;

    text_start = (width - text_len) / 2;
    memcpy(line_buffer + text_start, text, text_len);

    return line_buffer;
}

static int parse_articles_response(char *input, struct Article *articles, int capacity)
{
    int count = 0;
    char *p = input;

    while (*p && count < capacity)
    {
        char *line = p;
        char *nl = strchr(p, '\n');
        char *comma;
        int i;

        if (nl) { *nl = 0; p = nl + 1; }
        else    { p += strlen(p); }

        comma = strchr(line, ',');
        if (!comma) continue;

        *comma++ = 0;
        if (*comma == ' ') comma++;

        i = 0;
        while (i < 80 && line[i]) { articles[count].title[i] = line[i]; i++; }
        articles[count].title[i] = 0;

        i = 0;
        while (i < 9 && comma[i]) { articles[count].id[i] = comma[i]; i++; }
        articles[count].id[i] = 0;

        count++;
    }

    return count;
}

static ArticlesState articles_reset(void)
{
    articles_page = 1;
    article_cursor_pos = 0;
    article_cursor_pos_prev = 0;

    rows = 21;
    articles_per_page = 18;

    memset(_articles, 0, MAX_ARTICLES_PER_PAGE * sizeof(struct Article));

    return ARTICLES_FETCH;
}

static ArticlesState articles_fetch(void)
{
    strcpy(fetching_buf, "FETCHING ARTICLES, PLEASE WAIT.");
    show_fetching_msg(true);
    articles_buffer = fetch_data(true);
    return ARTICLES_DISPLAY;
}

static ArticlesState articles_display(void)
{
    int i;
    unsigned char y;

    screen_clear();
    hd_bar(0, centered_line(search_term, screen_cols), ATTRIBUTE_HEADER);

    articles_on_this_page = parse_articles_response(articles_buffer, _articles, MAX_ARTICLES_PER_PAGE);

    for (i = 0; i < articles_on_this_page; i++)
    {
        y = headline_locations[i];
        screen_puts(0, y, ATTRIBUTE_NORMAL, articles_display_headline(_articles[i].title));
    }

    bar_set(headline_locations[0], 0, articles_on_this_page, article_cursor_pos);

    return ARTICLES_MENU;
}

static ArticlesState articles_menu(void)
{
    screen_fill_line(22, ATTRIBUTE_STATUS);
    screen_puts_center(22, ATTRIBUTE_STATUS, "UP/DOWN Choose  ESC Search  ENTER View");

    switch (cgetc())
    {
    case KEY_ESCAPE:
        return ARTICLES_EXIT;
    case ARROW_UP:
        return ARTICLES_UP;
    case ARROW_DOWN:
        return ARTICLES_DOWN;
    case ENTER:
        return ARTICLES_SELECT;
    }

    return ARTICLES_MENU;
}

static ArticlesState articles_up(void)
{
    if (article_cursor_pos > 0)
    {
        article_cursor_pos--;
        bar_set(headline_locations[0], 0, articles_on_this_page, article_cursor_pos);
    }
    return ARTICLES_MENU;
}

static ArticlesState articles_down(void)
{
    if (article_cursor_pos < articles_on_this_page - 1)
    {
        article_cursor_pos++;
        bar_set(headline_locations[0], 0, articles_on_this_page, article_cursor_pos);
    }
    return ARTICLES_MENU;
}

State articles(void)
{
    articlesState = ARTICLES_RESET;

    while (state == ARTICLES)
    {
        switch (articlesState)
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
