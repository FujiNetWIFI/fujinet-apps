/**
 * @brief   Command Handler
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>
#include "cmd.h"
#include "cd.h"
#include "dir.h"

extern uint8_t default_device;

void uppercase(char *s)
{
    while (*s)
    {
        *s = toupper(*s);
        s++;
    }
}

bool is_drive_change(char *t)
{
    if (t[0] == 'N')
        if (t[strlen(t)-1] == ':')
            return true;

    return false;
}

bool cmd(char *cmd, char *t)
{
    uppercase(t);

    if (is_drive_change(t))
    {
        default_device=(uint8_t)atoi(&t[1]);
        return true;
    }

    if (!strcmp(t,"CLS"))
        cls(1);
    else if (!strcmp(t,"CD"))
        cd(strtok(NULL, " "));
    else if (!strcmp(t,"DIR"))
        dir(strtok(NULL, " "),false);
    else if (!strcmp(t,"LIST"))
        dir(strtok(NULL, " "),true);
    
    return true;
}
