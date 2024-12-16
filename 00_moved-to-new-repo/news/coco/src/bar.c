/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Bar routines
 */

#include <cmoc.h>
#include <coco.h>
#include "bar.h"

/**
 * @brief toggle bar for Y position (0-15) 
 * @param y vertical position (0-15) 
 */
void bar(int y)
{
    unsigned char *sp = (unsigned char *)0x0400;
    int o = y << 5;

    sp += o;

    for (int i=0;i<32;i++)
    {
        // Only change non-semigraphics characters
        if (*sp < 0x80)
            *sp ^= 0x40;
        sp++;
    }
}

/**
 * @brief draw shadow for color c at vert pos y
 * @param y Vertical position (0-15)
 * @param c bkg color (0-7) 
 */
void shadow(int y, int c)
{
    unsigned char *sp = (unsigned char *)0x400;
    int o = y << 5;

    sp += o;

    *sp = (unsigned char)c | 0x0b;
    memset(sp+1,c | 0x03, 31);
}
