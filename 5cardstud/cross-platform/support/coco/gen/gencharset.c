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
#include <sys/stat.h>
#include "charset.h"


void main(void)
{
    int i;
    int f = open("charset",O_CREAT|O_TRUNC|O_RDWR);

    // Ensure the file is accessible
    fchmod(f,777);
    
    unsigned char c = 0;

    for(i=0; i<sizeof(charset); ++i)
    {
        c |= (charset[i] == '*') << (7-(i%8));
        if (i%8 == 7)
        {
            write(f,&c,1);
            c = 0;
        }
    }

    close(f);
}
