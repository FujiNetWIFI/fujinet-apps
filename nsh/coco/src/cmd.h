/**
 * @brief   Command Handler
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#ifndef CMD_H
#define CMD_H

#include "stdbool.h"

/**
 * @brief handle commands
 * @param cmd The command line
 * @param t The next token
 */
bool cmd(char *cmd, char *t);

#endif /* CMD_H */
