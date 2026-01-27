/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Bar routines
 */

#include <cmoc.h>
#include <coco.h>
#include "globals.h"
#include "cocotext.h"
#include "bar.h"

void hd_bar(byte y, const char *text, bool rev)
{
    gotoxy(0, y);
    int width;

    if (hirestxt_mode)
    {
        width = textMode;
    }
    else
    {
        width = textMode - 1;
    }

    reverse(rev);

    printf("%-*s", width, text);

    reverse(false);
}

void multiline_hd_bar(byte y, int lines, const char *text, bool rev)
{
    int width;

    gotoxy(0, y);

    if (hirestxt_mode)
    {
        width = textMode;
    }
    else
    {
        width = textMode -1;
    }

    reverse(rev);

    printf("%-*s", (width * lines), text);

    reverse(false);
}



