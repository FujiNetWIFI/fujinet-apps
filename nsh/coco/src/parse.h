/**
 * @brief   Command Line Parser
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#ifndef CMD_H
#define CMD_H

#include "stdbool.h"

/**
 * @brief parse command line, and hand off to command handler
 */
bool parse(void);

#endif /* CMD_H */
