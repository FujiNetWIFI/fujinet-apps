/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Global State variables
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <cmoc.h>
#include <coco.h>

#define true 1
#define false 0

#define ARROW_UP 0x5E
#define ARROW_DOWN 0x0A
#define ARROW_LEFT 0x08
#define ARROW_RIGHT 0x09
#define ENTER 0x0D
#define BREAK 0x03

#define FG_BLACK 0
#define FG_GREEN 1
#define FG_WHITE 3
#define BG_GREEN 0
#define BG_BLUE 2
#define BG_RED 4

#define isprint(c) (c>=0x20 && c<=0x8E)
#undef isdigit
#define isdigit(c) (c>=0x30 && c<=0x39)

/**
 * @brief available program states
 * @enum SEARCH get search term
 * @enum ARTICLES articles for given search
 * @enum ARTICLE The currently selected article
 * @enum BYE exiting program
 * @enum HALT program halted.
 */
typedef enum _state
{
    SEARCH = 0,
    ARTICLES,
    ARTICLE,
    BYE,
} State;

/**
 * @brief the current program state 
 */
extern State state;

/**
 * @brief Current articles page
 */
extern int articles_page;

/**
 * @brief Number of articles per page
 */
extern int articles_per_page;

/**
 * @brief the currently selected article ID 
 */
extern char *article_id;

/**
 * @brief current article page
 */
extern int article_page;

/**
 * @brief Buffer for fetching status display
 */
extern char fetching_buf[];

/**
 * @brief Number of rows of text to fetch
 */
extern int rows;

/**
 * @brief Current search term
 */
extern char search_term[];

/**
 * @brief The text mode - 32, 42, 51, 40, or 80
 */
extern unsigned char textMode;

/**
 * @brief The line where the menu goes
*/
extern unsigned char menu_line;

/**
 * @brief Determine if string is numeric
 * @param s The string to test
 * @return true if the string is numeric, false otherwise
 */
BOOL is_numeric(const char *s);

#endif /* GLOBALS_H */
