/**
 * @brief   NSH
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose The prompt
 */

#ifndef NSH_H
#define NSH_H

#include "stdbool.h"

/**
 * @brief Ask for user input
 * @return true if non-empty line, otherwise false
 */
bool nsh(void);

#endif /* NSH_H */
