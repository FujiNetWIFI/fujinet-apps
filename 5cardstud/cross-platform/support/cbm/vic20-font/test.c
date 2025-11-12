/**
 * @brief Display test
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <vic20.h>
#include <cbm.h>
#include <string.h>
#include "data.h"

#pragma bss-name (push,"CHR")
unsigned char csram[2048];
#pragma bss-name (pop)

/**
 * @brief set up charset
 */
void video_setup(void)
{
    memcpy(csram,charset,sizeof(charset));
    VIC.addr = 0xFD; // screen memory at 0x1E00, character RAM at 0x1400
}

void main(void)
{
    VIC.bg_border_color = 0x1D;
    video_setup();
    memcpy((unsigned char *)0x1E00,screen,sizeof(screen));
    memcpy((unsigned char *)COLOR_RAM,color,sizeof(color));
    while(1);
}
