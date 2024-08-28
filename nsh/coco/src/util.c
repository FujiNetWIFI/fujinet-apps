/**
 * @brief   Utility functions
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>
#include "stdbool.h"

/**
 * @brief Does string have N: devicespec?
 * @param d Devicespec to check
 * @return true/false.
 */
bool has_n(char *d)
{
    if (d[0]=='N' || d[0]=='n')
        if (strrchr(d,':'))
            return true;

    return false;
}
