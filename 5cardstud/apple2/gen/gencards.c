/**
 * @brief   Generate card bitmaps
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * bitmaps, 21 pixels across
 * split across 3 seperate bitmaps
 */
const char cards[]=
  "*******              "
  "*******              "
  "*******              "
  "*******              "
  "*******              "
  "*******              "
  "*******              "
  "*******              ";

void main(void)
{
    int i;
    int f = open("../src/cards.bin",O_CREAT|O_TRUNC|O_WRONLY);
    char c = 0;

    for(i=0; i<sizeof(cards); ++i)
    {
        c |= (cards[i] == '*') << i%7;
        if (i%7 == 6)
        {
            write(f,&c,1);
            c = 0;
        }
    }

    close(f);
}
