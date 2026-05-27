/**
 * @brief Simple test for the CoCo 3 VT100 engine.
 */

#include <cmoc.h>
#include <coco.h>
#include "vt100.h"
#include "screen.h"

/* Helper to feed a string through the VT100 decoder */
static void feed(const char *s)
{
    while (*s)
        vt100(*s++);
}

int main(void)
{
    unsigned char k;
    initCoCoSupport();

    /* The screen driver specifically targets the CoCo 3 GIME 80-column mode */
    if (!isCoCo3)
    {
        putstr("REQUIRES A COCO 3.\r", 19);
        return 1;
    }

    /* 1. Initialize hardware: 80 cols, palettes, and clear shadow buffer */
    screen_init();

    /* Screen initialization: prompt for monitor type */
    feed("\x1b[2J\x1b[HMONITOR TYPE?\r\n\r\n");
    feed("  R = RGB (DEFAULT)\r\n");
    feed("  C = COMPOSITE\r\n");
    screen_flush();

    while ((k = inkey()) == 0) ;
    if (k == 'C' || k == 'c')
        screen_palette(1);

    /* 2. Clear screen and home the cursor using VT100 sequences */
    feed("\x1b[2J\x1b[H");

    /* 3. Sample text with various attributes (SGR) */
    feed("\x1b[1mVT100 ENGINE TEST UTILITY\x1b[0m\r\n");
    feed("-------------------------\r\n\r\n");

    feed("NORMAL TEXT\r\n");
    feed("\x1b[1mBOLD (BRIGHT) TEXT\x1b[0m\r\n");
    feed("\x1b[4mUNDERLINE TEXT\x1b[0m\r\n");
    feed("\x1b[5mBLINKING TEXT\x1b[0m\r\n");
    feed("\x1b[7mINVERSE TEXT\x1b[0m\r\n\r\n");

    /* 4. ANSI foreground and background colors */
    feed("FOREGROUNDS: ");
    feed("\x1b[31mRED \x1b[32mGRN \x1b[33mYEL \x1b[34mBLU \x1b[35mMAG \x1b[36mCYN\x1b[0m\r\n");
    feed("BACKGROUNDS: ");
    feed("\x1b[41;37m RED \x1b[42;37m GRN \x1b[44;37m BLU \x1b[45;37m MAG \x1b[46;37m CYN \x1b[0m\r\n\r\n");

    /* 5. Direct positioning (CUP - Cursor Position) [Row;ColH] (1-based) */
    feed("\x1b[12;10HRow 12, Col 10");
    feed("\x1b[14;40HRow 14, Col 40");
    feed("\x1b[16;60HRow 16, Col 60");

    /* 6. Exit message at the bottom */
    feed("\x1b[24;1H\x1b[7m Press any key to exit... \x1b[0m");

    /* Flush results to hardware screen */
    screen_flush();

    /* Wait for a keypress */
    while (inkey() == 0);

    /* Return to standard 32-column mode before exiting */
    screen_shutdown();
    return 0;
}
