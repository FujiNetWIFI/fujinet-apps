/**
 * @brief   Apple2 Text I/O
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Uses Apple2 ROM routines. Thanks to Oliver Schmidt.
 */

#include <apple2.h>
#include <ctype.h>
#include <stdbool.h>

// The following pragmas temporarily disable the optimizer
// as the optimizer will even try to optimize in-line assembler.
#pragma optimize (push, off)
#pragma warn (unused-param, push, off)

bool is_2e = false;

/**
 * @brief called to initialize 80 column using PR#3
 */
void init80(void)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("jsr $C300"); // Call PR#3
    __asm__("sta $C080"); // page in RAM
}

/**
 * @brief Get character into return
 * @return key pressed
 */
unsigned char __fastcall__ inc(void)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("jsr $FD35"); // Key into A
    __asm__("and #$7F");  // Mask strobe bit.
    __asm__("sta $C080"); // page in RAM
}

/**
 * @brief internal wrapper to call COUT
 */
static void __fastcall__ out(char c)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("ora #$80");  // set high bit in c
    __asm__("jsr $FDED"); // call COUT
    __asm__("sta $C080"); // page in RAM
}

/**
 * @brief place cursor character at position via COUT 
 */
void __fastcall__ place_cursor(void)
{
    __asm__("sta $C082"); // page in ROM
    __asm__("lda #$20");  // Cursor character $20
    __asm__("jsr $FDED"); // Call COUT
    __asm__("sta $C080"); // page in RAM
}

// Restore previous optimizer settings
#pragma warn (unused-param, pop)
#pragma optimize (pop)

/**
 * @brief Check if 80 column
 */
void check80column(void)
{
    is_2e = get_ostype() >= APPLE_IIE;
}

/**
 * @brief return if 80 column detected
 */
bool is80column(void)
{
    return is_2e;
}

/**
 * @brief Output a character using COUT
 * @param c Character to output
 */
void outc(char c)
{
    if (!is_2e)
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
  while (*s) {
    outc(*s++);
  }
}
