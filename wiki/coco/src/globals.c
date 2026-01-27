/**
 * @brief News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Global Variables
 */

#include "globals.h"

/**
 * @brief Program state
 */
State state;

/**
 * @brief Current text mode
*/
unsigned char textMode = 42;

/**
 * @brief The line where the menu goes
*/
unsigned char menu_line = 23;

/**
 * @brief Current articles page
 */
int articles_page;

/**
 * @brief Number of articles per page
 */
int articles_per_page;

/**
 * @brief the currently selected article ID 
 */
char *article_id;

/**
 * @brief current article page
 */
int article_page;

/**
 * @brief Buffer for fetching status display
 */
char fetching_buf[81];

/**
 * @brief Number of rows of text to fetch
 */
int rows;

/**
 * @brief Current search_term
 */
char search_term[115];

/**
 * @brief Determine if string is numeric
 * @param s The string to test
 * @return true if the string is numeric, false otherwise
 */
BOOL is_numeric(const char *s)
{
    if (*s == '\0')
        return FALSE;   // empty string is not numeric

    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return FALSE;
        s++;
    }
    return TRUE;
}
