/**
 * @brief   FujiNet News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Articles Display/Select
 */

#include <cmoc.h>
#include <coco.h>
#include "globals.h"
#include "articles.h"
#include "net.h"
#include "bar.h"

#define ARTICLES_PER_PAGE 3

static ArticlesState articlesState = ARTICLES_RESET;
static int articles_page = 1;
static char _articles[1024];
static char outs[97];

extern long article_id;

long article_ids[3]={0L,0L,0L};

static const char *topic_titles[9] =
    {
        "          TOP STORIES           ",
        "           WORLD NEWS           ",
        "            BUSINESS            ",
        "            SCIENCE             ",
        "           TECHNOLOGY           ",
        "             HEALTH             ",
        "         ENTERTAINMENT          ",
        "            POLITICS            ",
        "             SPORTS             "
    };

static char articles_on_this_page = 0;
static char *page = NULL; // Strtok ptr for page delimiter.
static char article_cursor_pos = 0;
static char article_cursor_pos_prev = 0;

const char *category_name_to_num(int c)
{
    switch(c)
    {
    case 0:
        return "top";
    case 1:
        return "world";
    case 2:
        return "business";
    case 3:
        return "science";
    case 4:
        return "technology";
    case 5:
        return "health";
    case 6:
        return "entertainment";
    case 7:
        return "politics";
    case 8:
        return "sports";
    }
}

/**
 * @brief Reset articles state
 */
ArticlesState articles_reset(void)
{
    articles_page = 1;
    return ARTICLES_FETCH;
}

/**
 * @brief Fetch articles for current page
 */
ArticlesState articles_fetch(void)
{
    char url[256];
    NetworkStatus ns;

    memset(_articles,0,sizeof(_articles));
    
    cls(2);
    printf("FETCHING ARTICLES, PLEASE WAIT. ");

    // Set up URL
    sprintf(url,"%s?t=lf&ps=32x13&l=%u&p=%u&c=%s",
            urlBase,
            ARTICLES_PER_PAGE,
            articles_page,
            category_name_to_num(selectedTopic));
    
    net_open(0,12,0,url);
    net_status(0,&ns);
    net_read(0,(byte *)&_articles[0],ns.bytesWaiting);
    net_close(0);
    
    return ARTICLES_DISPLAY;
}

/**
 * @brief Transform and display article headline
 * @param s headline string
 */
char *articles_display_headline(char *s)
{
    memset(outs,0,sizeof(outs));

    strncpy(outs,s,96);

    // Ellipsize if needed.
    if (strlen(outs)==96)
    {
        outs[93]=outs[94]=outs[95]='.';
    }

    return strupr(outs);
}

/**
 * @brief Display fetched articles
 */
ArticlesState articles_display(void)
{
    int i=0;

    locate(0,0);

    articles_on_this_page=0;
    
    // Go ahead and get page indicator
    page = strtok(_articles,"\n");

    // Start tokenizing the article list, we fetched.
    char *p = strtok(NULL,delim);

    while (p != NULL)
    {
        char stamp[33];

        memset(&stamp[0],0x80,33);
        stamp[32]=0x00;
        
        article_ids[i++] = atol(p);

        p = strtok(NULL, delim);
        memcpy(&stamp[13],p,19);
        printf("%32s",stamp);

        p = strtok(NULL, "\n");
        printf("%-96s",articles_display_headline(p));

        p = strtok(NULL,delim);
    }


    shadow(12,0x90);

    locate(0,13);
    printf("%s",topic_titles[selectedTopic]);

    locate(0,13);
    printf("%s",page);

    bar(13);

    bar(article_cursor_pos*4+1);
    bar(article_cursor_pos*4+2);
    bar(article_cursor_pos*4+3);
    
    return ARTICLES_MENU;
}

/**
 * @brief Update articles menu bar
 */
void articles_bar(void)
{
    if (article_cursor_pos_prev != article_cursor_pos)
    {
        bar(article_cursor_pos_prev*4+1);
        bar(article_cursor_pos_prev*4+2);
        bar(article_cursor_pos_prev*4+3);
        bar(article_cursor_pos*4+1);
        bar(article_cursor_pos*4+2);
        bar(article_cursor_pos*4+3);
        article_cursor_pos_prev = article_cursor_pos;
    }
}

/**
 * @brief Handle menu for current article page
 */
ArticlesState articles_menu(void)
{

    locate(0,14);
    printf("  up/dn CHOOSE    break TOPICS    lf/rt PAGE      return VIEW  ");
    
    articles_bar();
    locate(31,15);

    switch(waitkey(1))
    {
    case 0x03:
        return ARTICLES_EXIT;
    case 0x5E:
        return ARTICLES_UP;
    case 0x0A:
        return ARTICLES_DOWN;
    case 0x0D:
        return ARTICLES_SELECT;
    case 0x08:
        return ARTICLES_PREV_PAGE;
    case 0x09:
        return ARTICLES_NEXT_PAGE;
    }
    
    return ARTICLES_MENU;
}

/**
 * @brief handle moving up, for current articles page
 */
ArticlesState articles_up(void)
{
    if (article_cursor_pos)
    {
        articles_bar();
        article_cursor_pos--;
    }
    else
    {
        return ARTICLES_PREV_PAGE;
    }
    
    return ARTICLES_MENU;
}

/**
 * @brief handle moving down, for current articles page
 */
ArticlesState articles_down(void)
{
    if (article_cursor_pos<2)
    {
        articles_bar();
        article_cursor_pos++;
    }
    else
    {
        return ARTICLES_NEXT_PAGE;
    }
    
    return ARTICLES_MENU;
}

/**
 * @brief Move to previous article page, if possible.
 */
ArticlesState articles_prev_page(void)
{
    if (articles_page>1)
        articles_page--;

    article_cursor_pos=article_cursor_pos_prev=2;
    
    return ARTICLES_FETCH;
}

/**
 * @brief Move to next article page, if possible.
 */
ArticlesState articles_next_page(void)
{
    articles_page++;

    article_cursor_pos=article_cursor_pos_prev=0;

    return ARTICLES_FETCH;
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
            article_id = article_ids[article_cursor_pos];
            return ARTICLE;
        case ARTICLES_EXIT:
            return TOPICS;
        case ARTICLES_PREV_PAGE:
            articlesState = articles_prev_page();
            break;
        case ARTICLES_NEXT_PAGE:
            articlesState = articles_next_page();
            break;
        }
    }

    return TOPICS;
}
