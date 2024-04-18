/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Topics
 */

#ifndef TOPICS_H
#define TOPICS_H

#include "globals.h"

typedef enum _topic_state
{
    TOPICS_RESET = 0,
    TOPICS_DISPLAY,
    TOPICS_MENU,
    TOPICS_UP,
    TOPICS_DOWN,
    TOPICS_SELECT,
    TOPICS_EXIT,
} TopicState;

extern TopicState topicState;

/**
 * @brief Handle Topics menu
 */
State topics(void);

#endif /* TOPICS_H */
