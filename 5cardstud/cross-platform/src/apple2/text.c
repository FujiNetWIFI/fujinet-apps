/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#include "hires.h"
#include "charset.h"

#define CHAR_HEIGHT             8
#define SCREEN_WIDTH            40
#define SCREEN_HEIGHT           192

#define BOTTOM 175

extern unsigned char charset[];
unsigned char rop_mod[] = {0x55,0x2A};

/*-----------------------------------------------------------------------*/
void hires_putc(unsigned char x, unsigned char y, unsigned rop, unsigned char c)
{
    hires_Draw(x,y,1,CHAR_HEIGHT,rop,&charset[c<<3]);
}

/*-----------------------------------------------------------------------*/
void hires_puts(unsigned char x, unsigned char y,unsigned rop, char *s)
{
    while(*s)
      hires_putc(x++,y,rop|rop_mod[x%2],*s++);
}

/*-----------------------------------------------------------------------*/
void hires_putcc(unsigned char x, unsigned char y,unsigned rop, unsigned cc)
{
  hires_putc(x,y,rop,cc>>8);
  hires_putc(++x,y,rop,cc);
}