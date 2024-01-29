/**
 * @brief petsciiterm main
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

extern void directory(); // directory.c
extern void term();      // term.c
extern void ihsetup();   // srq.s

void main (void)
{
    ihsetup();
    while (1)
    {
        directory();
        term();
    }
}
