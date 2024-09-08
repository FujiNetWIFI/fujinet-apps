/**
 * @brief   Change Directory
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>
#include <fujinet-network.h>
#include "util.h"

extern uint8_t default_device;

void cd(char *t)
{
    char cd_buf[256];
    
    if (!has_n(t))
        sprintf(cd_buf,"N%u:%s",default_device,t);
    else
        strcpy(cd_buf,t);
    
    network_fs_cd(cd_buf);
}
