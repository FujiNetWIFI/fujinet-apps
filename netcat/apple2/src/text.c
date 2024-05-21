/**
 * @brief   Apple2 Text I/O
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Uses Apple2 ROM routines. Thanks to Oliver Schmidt.
 */

#include <apple2.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#include "text.h"

// The following pragmas temporarily disable the optimizer
// as the optimizer will even try to optimize in-line assembler.
#pragma optimize (push, off)
#pragma warn (unused-param, push, off)

/**
 * @brief called to initialize 80 column using PR#3
 */
void init80(void)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("jsr $C300"); // call PR#3
    __asm__("sta $C080"); // page in RAM
}

/**
 * @brief internal wrapper to call COUT
 */
static void __fastcall__ out(char c)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("eor #$80");  // invert high bit in c
    __asm__("jsr $FDED"); // call COUT
    __asm__("sta $C080"); // page in RAM
}

// Restore previous optimizer settings
#pragma warn (unused-param, pop)
#pragma optimize (pop)

/**
 * @brief return if 80 column detected
 */
bool has80column(void)
{
    return *(uint8_t *)0xBF98 /* MACHID */ & 0x02 ? true : false;
}

/**
 * @brief Output a character using COUT
 * @param c Character to output
 */
void outc(char c)
{
    if (!has80column())
    {
        c = toupper(c); // upper it.
    }
    out(c == '\n' ? '\r' : c);
}

/**
 * @brief send character string using outc()
 * @param s The NULL-terminated string to send through outc()
 */
void outs(const char *s)
{
    while (*s)
    {
        outc(*s++);
    }
}

/**
 * @brief place cursor character at position via inverted blank 
 */
void place_cursor(void)
{
    outc(' ' | 0x80);
}

/**
 * @brief remove cursor character at position via BS + blank + BS
 */
void remove_cursor(void)
{
    outc(CH_CURS_LEFT);
    outc(' ');
    outc(CH_CURS_LEFT);
}
