#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "article.h"
#include "screen.h"
#include "network.h"

static char *article_page_buffer;
static char *title = NULL;
static char *page = NULL;
static char *pageData = NULL;
static int page_max = 0;
static ArticleState articleState;
static char menu_line_buffer[81];

static char *article_display_headline(char *s)
{
    static char buf[81];
    int len = screen_cols;

    memset(buf, 0, sizeof(buf));
    if (len > 80) len = 80;
    strncpy(buf, s, len);

    if ((int)strlen(buf) == len)
    {
        buf[len - 3] = '.';
        buf[len - 2] = '.';
        buf[len - 1] = '.';
    }

    return buf;
}

static void parse_article_response(char *input)
{
    char *p;
    char *slash;

    if (!input)
        return;

    title = input;

    p = strchr(input, '\n');
    if (!p)
        return;

    *p = '\0';
    p++;

    page = p;
    page += 3;

    p = strchr(p, '\n');
    if (!p)
        return;

    *p = '\0';
    p++;

    pageData = p;

    slash = strchr(page, '/');
    if (slash)
        page_max = atoi(slash + 1);
    else
        page_max = 0;
}

static ArticleState article_reset(void)
{
    article_page = 1;
    rows = 19;
    return ARTICLE_FETCH;
}

static ArticleState article_fetch(void)
{
    strcpy(fetching_buf, "FETCHING PAGE, PLEASE WAIT.");
    show_fetching_msg(true);
    article_page_buffer = fetch_data(false);
    parse_article_response(article_page_buffer);
    return ARTICLE_DISPLAY;
}

static ArticleState article_display(void)
{
    unsigned char x, y;

    screen_clear();
    hd_bar(0, article_display_headline(title), ATTRIBUTE_HEADER);

    x = 0;
    y = 2;
    if (pageData)
    {
        char *p = pageData;
        while (*p)
        {
            if (*p == '\n')
            {
                x = 0;
                y++;
            }
            else
            {
                if (x < screen_cols && y < 22)
                    screen_putc(x, y, ATTRIBUTE_NORMAL, *p);
                x++;
            }
            p++;
        }
    }

    return ARTICLE_MENU;
}

static ArticleState article_menu(void)
{
    memset(menu_line_buffer, 0, sizeof(menu_line_buffer));
    sprintf(menu_line_buffer, "%-7s  UP/DOWN Page  ESC Articles", page ? page : "");

    screen_fill_line(23, ATTRIBUTE_STATUS);
    screen_puts(0, 23, ATTRIBUTE_STATUS, menu_line_buffer);

    switch (cgetc())
    {
    case KEY_ESCAPE:
        return ARTICLE_EXIT;
    case ARROW_UP:
        return ARTICLE_PREV_PAGE;
    case ARROW_DOWN:
        return ARTICLE_NEXT_PAGE;
    }

    return ARTICLE_MENU;
}

static ArticleState article_next_page(void)
{
    article_page++;

    if (article_page > page_max)
    {
        article_page--;
        return ARTICLE_MENU;
    }

    return ARTICLE_FETCH;
}

static ArticleState article_prev_page(void)
{
    if (article_page == 1)
        return ARTICLE_MENU;

    article_page--;

    return ARTICLE_FETCH;
}

State article(void)
{
    articleState = ARTICLE_RESET;

    while (state == ARTICLE)
    {
        switch (articleState)
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
