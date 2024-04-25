/**
 * @brief   Generate charset 
 * @author  Thomas Cherryhomes
 * @author  Oliver Schmidt
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "charset.h"

void main(void)
{
    int i;
    int f = open("charset",O_CREAT|O_TRUNC|O_RDWR);
    char c = 0;

    for(i=0; i<sizeof(charset); ++i)
    {
        c |= (charset[i] == '*') << i%7;
        if (i%7 == 6)
        {
            write(f,&c,1);
            c = 0;
        }
    }

    close(f);
}
