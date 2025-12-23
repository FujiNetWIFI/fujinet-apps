#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "4x6font.h"



SCB_REHV_PAL char_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,	
    0x0100, 0x0100,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }		// only two pens used, background, white
};


// static variables to reduce code size
unsigned char **font;
unsigned char xc;
char *c;


void _outtext_font(uint8_t fg, uint8_t bg)
{
  // initialize
  while (*c) {
    // Character in printable range?
    if ((*c > 0x20) && (*c < 0x7E)) {
        char_sprite.data = font[*c-0x20];
        char_sprite.hpos = xc;
        char_sprite.penpal[0] = (fg << 4) + (bg); 
    }
    else {
        char_sprite.data = font[0];         // space
        char_sprite.hpos = xc;
        char_sprite.penpal[0] = (bg << 4); 
    }
    
    tgi_sprite(&char_sprite);
    xc += 4;
    c++;
  }
}


// Output string at x, y screen location (Lynx screen coords)
void outtext_4x6(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg, char *s)
{
    c = s;
    xc = x;
    char_sprite.vpos = y; 

    // set font
    font = &font4x6[0];

    _outtext_font(fg, bg);
}


/*void outtext_4x8(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg, char *s)
{
    c = s;
    xc = x;
    char_sprite.vpos = y; 

    // set font
    font = &font4x8[0];

    _outtext_font(fg, bg);
}*/





/*void main (void)
{
  // Setup TGI
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_setdrawpage(0);
  tgi_setviewpage(0);
  tgi_clear();
  tgi_setcolor(TGI_COLOR_GREEN);
  tgi_bar(0, 0, 159, 101);
  tgi_setcolor(TGI_COLOR_WHITE);

  // setup joystick
  joy_install(joy_static_stddrv);


  chip_sprite.hpos = 1;
  chip_sprite.vpos = 73;
  tgi_sprite(&chip_sprite);

  outtext_4x6(6, 71, 0x0F, 0x00, "500");
  outtext_4x6(1, 77, 0x0F, 0x00, "CLYDE BOT");
  draw_card(1, 84, "5", SUIT_HEART);
  draw_card(10, 84, "K", SUIT_SPADE);
  draw_card(19, 84, "A", SUIT_DIAMOND);
  draw_card(28, 84, "3", SUIT_CLUB);
  draw_card(37, 84, "Q", SUIT_SPADE);
}*/