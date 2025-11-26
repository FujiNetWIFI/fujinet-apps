#ifdef _CMOC_VERSION_

#include <cmoc.h>
#include <coco.h>
#include "../platform-specific/sound.h"
#include "hires.h"
#include "../misc.h"

extern uint8_t charset[];

extern uint8_t xor_mask;
extern uint8_t font_shift;

/*-----------------------------------------------------------------------*/
void hires_putc(uint8_t x, uint8_t y, uint8_t rop, uint8_t c)
{
  hires_Draw(x,y,9,rop,&charset[(uint16_t)c<<3]);
}

/*-----------------------------------------------------------------------*/
void hires_putcc(uint8_t x, uint8_t y,uint8_t rop, uint16_t cc)
{
    // CMOC _REALLY_ hates this.
    hires_putc(x,y,rop,cc >> 8 & 0xFF); 
    hires_putc(++x,y,rop,cc & 0xFF);
}

void hires_Mask(uint8_t x, uint8_t y, uint8_t xlen, uint8_t ylen, uint8_t c)
{
  uint8_t *pos = (uint8_t *)SCREEN+(uint16_t)y*WIDTH+x;
  ylen++;
  while (--ylen) {
    memset(pos,c,xlen);
    pos+=WIDTH;
  }
}

void hires_Draw(uint8_t x, uint8_t y, uint8_t ylen, uint8_t rop, uint8_t *src)
{
  uint8_t *pos = (uint8_t *)SCREEN+(uint16_t)y*WIDTH+x;
  while (--ylen) {
    *pos=(*(src++)|rop);
    if (xor_mask)
      *pos^=*(src+86*8-1);

      
#ifdef COCO3    
    // Change black on green text to white on black for status bar
    if (font_shift)
        *pos=*pos^0b10101010 | *pos>>1;
#endif
    pos+=WIDTH;
  }
}

#endif
