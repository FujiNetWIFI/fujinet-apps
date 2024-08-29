/**
 * @brief   Show directory
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose ---
 */

#include <cmoc.h>
#include <coco.h>
#include "net.h"
#include "stdbool.h"
#include "util.h"

#define EOF 136
#define MODE_DIR 6
#define LONG 0x80

extern unsigned char default_device;

void dir(char *devicespec, bool list)
{
    uint16_t bw=0;
    uint8_t c=0;
    uint8_t err=0;
    char buf[512];
    char dir_devicespec[256];

    // If no N:, add it using the current default device.
    if (!has_n(devicespec))
        sprintf(dir_devicespec,"N%u:%s",default_device,devicespec);
    else
        strcpy(dir_devicespec,devicespec);
    
    if (list)
        list=LONG;
    
    if (!network_open(dir_devicespec,MODE_DIR,list))
    {
        printf("COULD NOT OPEN DIRECTORY\n");
        return;
    }

    network_status(dir_devicespec, &bw, &c, &err);

    while (err != EOF)
    {
        memset(buf,0,sizeof(buf));
        
        if (bw > sizeof(buf))
            bw=sizeof(buf);
        
        network_read(dir_devicespec, (unsigned char *)buf, bw);

        printf("%s",buf);
        
        network_status(dir_devicespec, &bw, &c, &err);
    }
    
 dir_close:
    network_close(dir_devicespec);
}
