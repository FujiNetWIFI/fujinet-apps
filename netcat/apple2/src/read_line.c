/**
 * @brief   Line Input routine
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Gosh I love writing these things, really, I do... #@(%#@)_%@
 */

#include <apple2.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief read an input line.
 * @param s The output buffer. Must be at least len bytes.
 * @param len Allowable # of characters.
 * @param password Obscure character output.
 * @return last character emitted.
 */
char read_line(char *s, uint16_t len, bool password)
{
    uint16_t i=0;
    char c=0;

    cursor(0);
    
    while (c != CH_ENTER)
    {
        revers(1);
        putchar(0x20);
        revers(0);
        c=cgetc();
        putchar(0x08);

        if ((c == CH_CURS_LEFT) || (c == CH_DEL))
        {
            if (i)
            {
                putchar(0x08);
                i--;
            }
        }
        else if (c == CH_ENTER)
        {
            continue;
        }
        else if (c == CH_CURS_RIGHT)
        {
            continue;
        }
        else if (c < 0x20 || c > 0x7E) // Non printable ignored.
        {
            continue;
        }
        else // print and append character in buffer.
        {
            if ((i+1) > len)
            {
                continue;
            }
            else
            {
                if (password)
                {
                    putchar('X');
                }
                else
                {    
                    putchar(c);
                }
                
                s[i++] = c;
            }
        }
    }

    return c;
}
