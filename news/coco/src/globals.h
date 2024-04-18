/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Global State variables
 */

#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * @brief available program states
 * @enum TOPICS current topics
 * @enum ARTICLES articles for given topic
 * @enum ARTICLE The currently selected article
 * @enum BYE exiting program
 * @enum HALT program halted.
 */
typedef enum _state
{
    TOPICS = 0,
    ARTICLES,
    ARTICLE,
    BYE,
    HALT
} State;

/**
 * @brief currently selected topic
*/
extern enum selected_topic selectedTopic;

/**
 * @brief the current program state 
 */
extern State state;

/**
 * @brief the base URL to be prepended.
 */
extern const char *urlBase;

/**
 * @brief The strtok() delimiter used for returned article lists
 */
extern const char *delim;

#endif /* GLOBALS_H */
