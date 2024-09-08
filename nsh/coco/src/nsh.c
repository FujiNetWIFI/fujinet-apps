/**
 * @brief   NSH
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose The main shell
 */

#include <cmoc.h>
#include <coco.h>
#include "stdbool.h"
#include <fujinet-network.h>

#define PROMPT_FORMAT "N%u:%s>"

char *cmdline = NULL;
unsigned char default_device = 1;
char default_devicespec[8];
char default_prefix[256];

void nsh_prompt(void)
{
    sprintf(default_devicespec,"N%u:",default_device);
    // net_pwd(default_devicespec,&default_prefix);
    printf("N%u:%s>",default_device,default_prefix);
}

bool nsh(void)
{
    nsh_prompt();
    cmdline = readline();

    if (!cmdline[0])
        return false;
    
    return true;
}
