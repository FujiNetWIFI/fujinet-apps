/**
 * @brief Test harness for the CoCo 3 vt100 rendering pipeline.
 *
 * Feeds a canned VT100 stream (attributes, cursor positioning, scrolling) to
 * the engine and flushes the shadow to the hardware screen. No network yet -
 * this validates screen.c + term.c + vt100.c on the direct-write mechanism.
 */

#include <cmoc.h>
#include <coco.h>
#include "vt100.h"
#include "screen.h"

static void wait_key(void)
{
    while (inkey()) ;
    while (!inkey()) ;
}

static void feed(const char *s)
{
    while (*s)
        vt100(*s++);
}

int main(void)
{
    char buf[48];
    int i;

    initCoCoSupport();
    if (!isCoCo3)
    {
        printf("NEEDS A COCO 3.\r");
        return 1;
    }

    screen_init();

    feed("\x1b[2J\x1b[H");
    feed("CoCo 3 VT100 engine -- direct 80-column text\r\n\r\n");
    feed("Attributes: Normal \x1b[7mInverse\x1b[0m "
         "\x1b[4mUnderline\x1b[0m \x1b[5mBlink\x1b[0m\r\n\r\n");
    feed("80-column ruler:\r\n");
    feed("0123456789012345678901234567890123456789"
         "0123456789012345678901234567890123456789\r\n\r\n");
    feed("\x1b[14;20HPositioned at row 14, col 20.");
    feed("\x1b[24;1HPress a key for the scroll test...");
    screen_flush();
    wait_key();

    feed("\x1b[2J\x1b[H");
    for (i = 1; i <= 30; i++)
    {
        /* keep \r\n out of formatting calls: cmoc translates \n at runtime */
        feed("scroll line ");
        itoa10(i, buf);
        feed(buf);
        feed(" - the quick brown fox jumps\r\n");
        screen_flush();
        delay(6);
    }
    feed("\x1b[7m Done. Press a key to exit. \x1b[0m");
    screen_flush();
    wait_key();

    width(32);
    return 0;
}
