/**
 * @brief   Command Line Parser
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>
#include "stdbool.h"
#include "cmd.h"

/**
 * @brief pointer to global readline buffer
 */
extern char *cmdline;

/**
 * @brief pointer to next parameter token (strtok())
 */
char *t = NULL;

bool parse(void)
{
    t = strtok(cmdline, " ");

    return cmd(cmdline,t);
}
