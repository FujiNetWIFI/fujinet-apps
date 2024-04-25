/**
 * @brief Text routines
 * @author Oliver Schmidt
 */

#include "hires.h"

#define CHAR_HEIGHT             8
#define SCREEN_WIDTH            40
#define SCREEN_HEIGHT           192

#define BOTTOM 175

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

void status(char *s)
{
  unsigned char i,j;
  
  for (i=0;i<40;i++)
    {
      hires_putc(i,175,ROP_AND(0x2A),' ');
      hires_putc(i,183,ROP_AND(0x2A),' ');
    }

  i=0;
  j=BOTTOM;
  
  while(*s)
    {
      if (*s=='\n')
	{
	  i=0;
	  j+=8;
	}
      else
	hires_putc(i++,j,ROP_CPY,*s);

      s++;
    }
}
