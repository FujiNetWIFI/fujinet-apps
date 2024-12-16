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
#include "topics.h"
#include "bar.h"

#define true 1
#define false 0
#define MENU_Y 3

#define ARROW_UP 0x5E
#define ARROW_DOWN 0x0A
#define ENTER 0x0D
#define BREAK 0x03

TopicState topicState = TOPICS_RESET;

enum _selected_topic
{
    TOP_STORIES = 0,
    WORLD_NEWS,
    BUSINESS,
    SCIENCE,
    TECHNOLOGY,
    HEALTH,
    ENTERTAINMENT,
    POLITICS,
    SPORTS
} selectedTopic = TOP_STORIES;

int topicChanged = 0; // flag for updating selection bar

/**
 * @brief Reset topics menu 
 */
TopicState topics_reset(void)
{
    selectedTopic = TOP_STORIES;
    topicChanged = 1;
    return TOPICS_DISPLAY;
}

TopicState topics_display(void)
{
    cls(3);
    locate(0,0);
    printf("      FUJINET  NEWS TOPICS      ");
    shadow(1,0xA0);
    bar(0);

    locate(0,3);
    
    printf(" \x99 TOP STORIES\n");
    printf(" \x9F WORLD NEWS\n");
    printf(" \x8F BUSINESS\n");
    printf(" \xAF SCIENCE\n");
    printf(" \xBF TECHNOLOGY\n");
    printf(" \xCF HEALTH\n");
    printf(" \xDF ENTERTAINMENT\n");
    printf(" \xEF POLITICS\n");
    printf(" \xFF SPORTS\n");

    shadow(0x0C,0xA0);
    
    locate(0,15);
    printf("SELECT WITH ARROWS, THEN ENTER.");

    return TOPICS_MENU;
}

TopicState topics_menu()
{
    if (topicChanged)
    {
        topicChanged=0;
        bar(selectedTopic+MENU_Y);
    }
    
    switch(waitkey(true))
    {
    case ARROW_UP:
        return TOPICS_UP;
    case ARROW_DOWN:
        return TOPICS_DOWN;
    case ENTER:
        return TOPICS_SELECT;
    case BREAK:
        return TOPICS_EXIT;
    }       
}

TopicState topics_up()
{
    if (selectedTopic > TOP_STORIES)
    {
        bar(selectedTopic+MENU_Y);
        selectedTopic--;
        topicChanged=1;
    }
    
    return TOPICS_MENU;
}

TopicState topics_down()
{
    if (selectedTopic < SPORTS)
    {
        bar(selectedTopic+MENU_Y);
        selectedTopic++;
        topicChanged=1;
    }

    return TOPICS_MENU;
}

State topics(void)
{
    topicState=TOPICS_RESET;
    
    while (state == TOPICS)
    {
        switch(topicState)
        {
        case TOPICS_RESET:
            topicState = topics_reset(); 
            break;
        case TOPICS_DISPLAY:
            topicState = topics_display();
            break;
        case TOPICS_MENU:
            topicState = topics_menu();
            break;
        case TOPICS_UP:
            topicState = topics_up();
            break;
        case TOPICS_DOWN:
            topicState = topics_down();
            break;
        case TOPICS_SELECT:
            return ARTICLES;
        case TOPICS_EXIT:
            return BYE;
        }
    }

    return BYE;
}
