#ifdef _CMOC_VERSION_

/**
 * @brief Text routines
 * @author Oliver Schmidt
 */
//#include <string.h>
#include "hires.h"
#include "hires.h"


void drawTextAt(unsigned char x, unsigned char y, char*s);

#define CHAR_HEIGHT             8
#define SCREEN_WIDTH            40
#define SCREEN_HEIGHT           192

#define BOTTOM 175

extern unsigned char charset[];
unsigned char rop_mod[] = {0x55,0x2A};

void hires_double_buffer_draw()
{
//  memcpy((void*)0x4000,(void*)0x2000,0x2000);
}

/*-----------------------------------------------------------------------*/
void hires_putc(unsigned char x, unsigned char y, unsigned rop, unsigned char c)
{
//hires_Draw(x,y,1,CHAR_HEIGHT,rop,&charset[c<<3]);
  drawTextAt(x,y,c);  
}

/*-----------------------------------------------------------------------*/
void hires_putcc(unsigned char x, unsigned char y,unsigned rop, unsigned cc)
{
  drawTextAt(x,y,cc>>8);
  drawTextAt(++x,y,cc);
}

#endif
