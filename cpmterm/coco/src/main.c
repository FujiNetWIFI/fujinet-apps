/**
 * @brief   CPM
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Dumb Terminal
 */

#include <cmoc.h>
#include <coco.h>
#include "cpm.h"

char rxBuf[2048];

void out()
{
    byte k=inkey();

    if (k)
    {
        cpm_write(&k,1);
    }
}

void in()
{
    unsigned short bw=0;

    bw = cpm_status();

    if (bw > sizeof(rxBuf))
        bw = sizeof(rxBuf);
    
    if (!bw)
        return;
    
    putchar('\x08'); // erase cursor
    
    cpm_read((byte *)rxBuf,bw);
    
    for (unsigned short i=0;i<bw;i++)
    {
        char c = rxBuf[i];
        
        if (c != 0x0A)
            putchar(c);
    }
    
    putchar(0xAF); // place cursor.
}

int main(void)
{
    cpm_boot();

    while(1)
    {
        out();
        in();
    }

    return 0;
}
