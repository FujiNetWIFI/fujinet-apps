/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Article display
 */

#include <cmoc.h>
#include <coco.h>
#include "globals.h"
#include "article.h"
#include "net.h"
#include "bar.h"
/**
 * @brief the currently selected article ID 
 */
long article_id;

/**
 * @brief current article page
 */
int article_page=1;

/**
 * @brief Article Page buffer 
 */
char article_page_buffer[1024];

/**
 * @brief Article metadata
 */
static char *title = NULL;
static char *date = NULL;
static char *source = NULL;
static char *page = NULL;
static char *pageData = NULL;
static int page_max = 0;

/**
 * @brief current state for article view
 */
static ArticleState articleState;

/**
 * @brief the Article URL
 */
static char article_url[256];

extern char *articles_display_headline(char *s);

/**
 * @brief Initial entrypoint, reset local state
 */
ArticleState article_reset(void)
{
    article_page = 1;
    return ARTICLE_FETCH;
}

/**
 * @brief Fetch current article page
 */
ArticleState article_fetch(void)
{
    NetworkStatus ns;
    char *p = NULL;
    char tmp[8];
    
    memset(article_url,0,sizeof(article_url));
    memset(article_page_buffer,0,sizeof(article_page_buffer));

    cls(3);
    printf("%-32s","FETCHING PAGE, PLEASE WAIT.");
    
    sprintf(article_url,"%s?t=lf&ps=31x10&p=%u&a=%lu",
            urlBase,
            article_page,
            article_id);

    net_open(0,12,0,article_url);
    net_status(0,&ns);
    net_read(0,(byte *)&article_page_buffer[0],ns.bytesWaiting);
    net_close(0);

    title = strtok(article_page_buffer,"\n");
    date = strtok(NULL,"\n");
    source = strtok(NULL,"\n");
    page = strtok(NULL,"\n");
    pageData = strtok(NULL,"\0");

    // copy, parse page counter so we can get max # of pages.
    strcpy(tmp,page);
    p = strtok(tmp,"/"); // skip over current page
    p = strtok(NULL,"\0"); // get last # as max pages.
    page_max = atoi(p); // set it.
    
    return ARTICLE_DISPLAY;
}

/**
 * @brief Display current article page
 */
ArticleState article_display(void)
{    
    cls(3);
    
    printf("%-96s\n",articles_display_headline(title));
    bar(0);
    bar(1);
    bar(2);
    shadow(3,0xa0);
    printf("%s",strupr(pageData));
    shadow(14,0xa0);
    return ARTICLE_MENU;
}

/**
 * @brief Handle article menu
 */
ArticleState article_menu(void)
{
    locate(0,15);
    printf("      up/dn iNFO break ARTICLES");
    locate(0,15);
    printf("%s",page);
    
    locate(31,15);
    switch(waitkey(1))
    {
    case 0x03:
        return ARTICLE_EXIT;
    case 0x5E:
        return ARTICLE_PREV_PAGE;
    case 0x0A:
        return ARTICLE_NEXT_PAGE;
    case 'I':
        return ARTICLE_INFO;
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
 * @brief Show Article Metadata
 */
ArticleState article_info(void)
{
    cls(6);

    printf("%-96s",articles_display_headline(title));
    bar(0);
    bar(1);
    bar(2);
    shadow(3,0xD0);

    locate(0,4);
    printf("%10s%-22s","date:",strupr(date));
    printf("%10s%-22s","source:",strupr(source));
    shadow(6,0xD0);

    locate(0,15);
    printf("      ANY KEY  TO CONTINUE     ");

    waitkey(1);
    
    return ARTICLE_DISPLAY;
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
        case ARTICLE_INFO:
            articleState = article_info();
            break;
        case ARTICLE_EXIT:
            return ARTICLES;            
        }
    }

    return ARTICLES;
}
