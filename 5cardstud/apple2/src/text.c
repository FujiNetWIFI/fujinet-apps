/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#include "hires.h"

#define CHAR_HEIGHT             8
#define SCREEN_WIDTH            40
#define SCREEN_HEIGHT           192

extern unsigned char charset[1024];

/*-----------------------------------------------------------------------*/
void hires_putc(char x, char y, unsigned rop, char c)
{
    hires_Draw(x,y,1,CHAR_HEIGHT,rop,&charset[c<<3]);
}

/*-----------------------------------------------------------------------*/
void hires_puts(char x, char y, unsigned rop, char *s)
{
    while(*s)
        hires_putc(x++,y,rop,*s++);
}
