/**
 * @brief   Line Input routine
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Gosh I love writing these things, really, I do... #@(%#@)_%@
 */

#include <ctype.h>
#include <conio.h>
#include <stdint.h>
#include <stdbool.h>
#include "text.h"

#include "read_line.h"

#define CH_DEL 0x7F

/**
 * @brief read an input line.
 * @param s The output buffer. Must be at least len bytes.
 * @param len Allowable # of characters (incl. \0).
 * @param password Obscure character output.
 */
void read_line(char *s, uint16_t len, bool password)
{
    uint16_t i = 0;
    char c;

    for (;;)
    {
        place_cursor();
        c = cgetc();
        remove_cursor();

        if (c == CH_ENTER)
        {
            s[i] = '\0';
            break;
        }
        else if ((c == CH_CURS_LEFT) || (c == CH_DEL))
        {
            if (i)
            {
                outc(CH_CURS_LEFT);
                --i;
            }
        }
        else if (isprint(c))
        {
            if (i < (len - 1))
            {
                if (password)
                {
                    outc('*');
                }
                else
                {    
                    outc(c);
                }
                s[i++] = c;
            }
        }
    }
}
