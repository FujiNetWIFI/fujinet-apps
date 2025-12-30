#include <lynx.h>
#include <tgi.h>
#include <ctype.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../platform-specific/graphics.h"
#include "drawfont.h"
#include "cardsprites.h"
#include "lynxfnio.h"


#define LYNX_STATUS_Y    96              // status bar line, in lynx screen coords

#define LYNX_LINE_COLOR   TGI_COLOR_YELLOW
#define LYNX_BOX_COLOR    TGI_COLOR_BROWN

//unsigned char dblbuffer;
//unsigned char drawpage;
unsigned char lynx_fg_color;
unsigned char lynx_bg_color;
unsigned char lynx_card_fringe_color;

unsigned char colorMode;
unsigned char always_render_full_cards = 0;


// card suits - may not need
#define SUIT_HEART    0
#define SUIT_DIAMOND  1
#define SUIT_SPADE    2
#define SUIT_CLUB     3

// Card, Chip and Suit Sprites
SCB_REHV_PAL card_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &card_up_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x0E, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL card_partial_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &halfcard_down_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x0E, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL facedown_card_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &card_down_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x05, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL chip_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    (unsigned char *) &small_chip_spr,
    0, 0,
    0x0100, 0x0100,
    { 0x2F, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

SCB_REHV_PAL suit_sprite = {
    BPP_4 | TYPE_NORMAL,
    REHV,
    0x01,
    0,
    NULL,
    0, 0,
    0x0100, 0x0100,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};


// Character to screen coords helpers
unsigned char _char_x_scr(unsigned char x)
{
  return (x*4);
}


unsigned char _char_y_scr(unsigned char y)
{
  return (y*6);
}


// Draw a card sprite at x, y screen location (Lynx screen coords)
void lynx_drawcard(uint8_t x, uint8_t y, char *rank, char suit)
{
  unsigned char tcol;


  card_sprite.hpos = x;
  card_sprite.vpos = y;
  card_sprite.penpal[0] = lynx_card_fringe_color;

  suit_sprite.hpos = x+3;
  suit_sprite.vpos = y+9;

  switch(suit) {
    case SUIT_HEART:
      suit_sprite.data = (unsigned char *) &heart_spr;
      suit_sprite.penpal[0] = 0x20;							// Red
      tcol = 0x02;											// Red
      break;
    case SUIT_DIAMOND:
      suit_sprite.data = (unsigned char *) &diamond_spr;
      suit_sprite.penpal[0] = 0x20;
      tcol = 0x02;
      break;
    case SUIT_SPADE:
      suit_sprite.data = (unsigned char *) &spade_spr;
      suit_sprite.penpal[0] = 0x10;							// Black
      tcol = 0x01;											// Black
      break;
    case SUIT_CLUB:
      suit_sprite.data = (unsigned char *) &club_spr;
      suit_sprite.penpal[0] = 0x10;
      tcol = 0x01;
      break;
  }

  tgi_sprite(&card_sprite);							// draw card outline and bg (always white)
  tgi_sprite(&suit_sprite);							// draw card suit sprite

  if (rank[0] == '1') {								// is this a 10?
    outtext_4x6(x+2, y+2, tcol, 0x00, "1");			// shift 10 over slightly
    outtext_4x6(x+5, y+2, tcol, 0x00, "0");
  }
  else
    outtext_4x6(x+2, y+2, tcol, 0x00, rank);		// bg 0x00 is transparent
}


void lynx_draw_partial_card(uint8_t x, uint8_t y, uint8_t flip)
{
    card_partial_sprite.hpos = x;
    card_partial_sprite.vpos = y;

    // Flip card for right side
    if (flip) {
        card_partial_sprite.hpos = (x+4);
        card_partial_sprite.sprctl0 |= HFLIP;
    }
    else {
        card_partial_sprite.sprctl0 &= ~HFLIP;
        card_partial_sprite.hpos = x;
    }

    tgi_sprite(&card_partial_sprite);
}


void lynx_draw_facedown_card(uint8_t x, uint8_t y)
{
    facedown_card_sprite.hpos = x;
    facedown_card_sprite.vpos = y;
    facedown_card_sprite.penpal[1] = (facedown_card_sprite.penpal[1] & 0x0F) | (lynx_card_fringe_color << 4);

    tgi_sprite(&facedown_card_sprite);
}


// Initialize TGI, and screen
void initGraphics()
{
    // Setup TGI
    tgi_install(tgi_static_stddrv);
    tgi_init();
    tgi_setcolor(TGI_COLOR_WHITE);

    // setup joystick - seems the best place to do this
    joy_install(joy_static_stddrv);

    // Init Fujinet - since we don't have Lynx in network-lib yet
    fnio_init();

    // setup for double buffering (double buffer disabled for now)
    //dblbuffer = 0;
    //drawpage = 0;
    tgi_setviewpage(0);
    tgi_setdrawpage(0);

    // Initial color mode
    colorMode = 0;
    setColorMode(colorMode);

    resetScreen();
}


// Reset the screen
void resetScreen()
{
    // clear the screen
    tgi_setcolor(lynx_bg_color);
    tgi_clear();
    tgi_bar(0, 0, 159, 101);
    tgi_setcolor(lynx_fg_color);
}


void enableDoubleBuffer()
{
  /*tgi_updatedisplay();
  drawpage = !drawpage;
  while (tgi_busy());

  dblbuffer = 1; */
}

void disableDoubleBuffer()
{
   /*tgi_setviewpage(drawpage);
   dblbuffer = 0;*/
}

void drawBuffer()
{
  /*if (dblbuffer) {
    tgi_updatedisplay();
    drawpage = !drawpage;
    while (tgi_busy());
  }*/
}


void waitvsync()
{
  unsigned long t;

  t = clock();
  while (t == clock());
}


// Draw cards at corners of screen
void drawBorder() {
    // draw aces in each corner
    lynx_drawcard(2, 2, "A", SUIT_SPADE);
    lynx_drawcard(148, 2, "A", SUIT_HEART);
    lynx_drawcard(2, 78, "A", SUIT_DIAMOND);
    lynx_drawcard(148, 78, "A", SUIT_CLUB);
}


// Draw 5CS logo/title
// FIXME: on Lynx, maybe we display a sprite with logo?
void drawLogo()
{
    tgi_outtextxy(52, 21, "FUJI NET");
    tgi_outtextxy(24, 33, "FIVE CARD STUD");
}


void clearStatusBar()
{
  tgi_setcolor(lynx_bg_color);
  tgi_bar(0, LYNX_STATUS_Y, 159, LYNX_STATUS_Y+6);
  tgi_setcolor(lynx_fg_color);
}


void drawStatusTextAt(unsigned char x, const char* s)
{
  outtext_4x6(_char_x_scr(x), LYNX_STATUS_Y, LYNX_LINE_COLOR, lynx_bg_color, (char *) s);
}


void drawStatusText(const char* s) {
  clearStatusBar();
  outtext_4x6(0, LYNX_STATUS_Y, LYNX_LINE_COLOR, lynx_bg_color, (char *) s);
}


void drawStatusTimer()
{
  // draw the timer icon
}


// For optional HI-RES Y enabled platforms
void drawTextAt(unsigned char x, unsigned char y, const char* s)
{
  outtext_4x6(_char_x_scr(x), _char_y_scr(y), lynx_fg_color, lynx_bg_color, (char *) s);
}


void drawText(unsigned char x, unsigned char y, const char* s)
{
  outtext_4x6(_char_x_scr(x), _char_y_scr(y), lynx_fg_color, lynx_bg_color, (char *) s);
}


void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
    unsigned char sx, sy;
    unsigned char suit;
    char rank[3];


    // Convert to screen coords
    sx = _char_x_scr(x);
    sy = _char_y_scr(y);

    // Hidden card should be drawn off-white/gray
    if (isHidden)
        card_sprite.penpal[1] = 0x40;      // gray
    else
        card_sprite.penpal[1] = 0xF0;      // white

    // Card face down?
    if (s[0] == '?') {
        lynx_draw_facedown_card(sx, sy);
        return;
    }

    // Draw partial top of card
    if (partial == PARTIAL_LEFT)
    {
      lynx_draw_partial_card(sx, sy, 0);
      return;
    }
    else if (partial == PARTIAL_RIGHT)
    {
      lynx_draw_partial_card(sx, sy, 1);
      return;
    }

    // Card Suit
    switch(s[1]) {
        case 'h':
            suit = SUIT_HEART;
            break;
        case 'd':
            suit = SUIT_DIAMOND;
            break;
        case 'c':
            suit = SUIT_CLUB;
            break;
        case 's':
            suit = SUIT_SPADE;
            break;
    }

    // Card rank
    rank[0] = s[0];
    if (rank[0] == 't') {
        strcpy(rank, "10");
    }
    else {
      rank[0] = toupper(rank[0]);
      rank[1] = '\0';
    }

    // Draw face up card
    lynx_drawcard(sx, sy, rank, suit);
}


void drawChip(unsigned char x, unsigned char y)
{
  chip_sprite.hpos = _char_x_scr(x);
  chip_sprite.vpos = _char_y_scr(y)+2;
  tgi_sprite(&chip_sprite);
}


void drawBlank(unsigned char x, unsigned char y)
{
  tgi_setcolor(lynx_bg_color);
  tgi_bar(_char_x_scr(x), _char_y_scr(y),_char_x_scr(x)+8, _char_y_scr(y)+12);
  tgi_setcolor(lynx_fg_color);
}


void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
    tgi_setcolor(LYNX_LINE_COLOR);
    tgi_line(_char_x_scr(x), _char_y_scr(y), _char_x_scr(x+w), _char_y_scr(y));
    tgi_setcolor(lynx_fg_color);
}


void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
    tgi_setcolor(lynx_bg_color);
    tgi_line(_char_x_scr(x), _char_y_scr(y), _char_x_scr(x+w), _char_y_scr(y));
    tgi_setcolor(lynx_fg_color);
}


void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
	// draw the box
    tgi_setcolor(LYNX_BOX_COLOR);
    //tgi_line(_char_x_scr(x)+2, _char_y_scr(y)+4, _char_x_scr(x+w)+4, _char_y_scr(y)+4);     	// top line
    //tgi_lineto(_char_x_scr(x+w)+4, _char_y_scr(y+h)+8);       								// right line
    //tgi_lineto(_char_x_scr(x)+2, _char_y_scr(y+h)+8);         								// bottom line
    //tgi_lineto(_char_x_scr(x)+2, _char_y_scr(y)+4);           								// left line

    tgi_line(_char_x_scr(x)+3, _char_y_scr(y)+4, _char_x_scr(x+w)+3, _char_y_scr(y)+4);     	// top
    tgi_line(_char_x_scr(x)+3, _char_y_scr(y+h)+8, _char_x_scr(x+w)+3, _char_y_scr(y+h)+8);    	// bottom
    tgi_line(_char_x_scr(x+w)+4, _char_y_scr(y)+5, _char_x_scr(x+w)+4, _char_y_scr(y+h)+7);     // right
    tgi_line(_char_x_scr(x)+2, _char_y_scr(y)+5, _char_x_scr(x)+2, _char_y_scr(y+h)+7);     	// left

	// round the edges
	tgi_setpixel(_char_x_scr(x)+3, _char_y_scr(y)+5);			// TL
	tgi_setpixel(_char_x_scr(x+w)+3, _char_y_scr(y)+5);			// TR
	tgi_setpixel(_char_x_scr(x+w)+3, _char_y_scr(y+h)+7);		// BR
	tgi_setpixel(_char_x_scr(x)+3, _char_y_scr(y+h)+7);			// BL

    tgi_setcolor(lynx_fg_color);
}


unsigned char cycleNextColor() {
  setColorMode((colorMode+1 ) % 4);
  return colorMode;
}


void setColorMode(unsigned char mode) {
  colorMode = mode;

  if (colorMode == 0) {						    // White text on green BG
    lynx_fg_color = TGI_COLOR_WHITE;
    lynx_bg_color = TGI_COLOR_GREEN;
    lynx_card_fringe_color = 0x01;
  } else if (colorMode == 1) {				// Black text on green BG
    lynx_fg_color = TGI_COLOR_BLACK;
    lynx_bg_color = TGI_COLOR_GREEN;
    lynx_card_fringe_color = 0x01;
  } else if (colorMode == 2) {				// White on BLUE BG
    lynx_fg_color = TGI_COLOR_WHITE;
    lynx_bg_color = TGI_COLOR_BLUE;
    lynx_card_fringe_color = 0x01;
  } else {
    lynx_fg_color = TGI_COLOR_WHITE;		// White on Black BG
    lynx_bg_color = TGI_COLOR_BLACK;
    lynx_card_fringe_color = TGI_COLOR_LIGHTBLUE;
  }
}


void platformStatusKeyLegend()
{
					      //0123456789012345678901234567890123456789
	drawStatusText("2: REFRESH  P: HELP  1: COLOR  1+2: NAME");
}


void platformStatusMenuKeys()
{
	drawBox(8, 4, 24, 7);
					        //0123456789012345678901
	drawText(10, 6,  "FLIP:    QUIT TABLE");
	drawText(10, 7,  "PAUSE:   HOW TO PLAY");
	drawText(10, 8,  "OPTION1: CHANGE COLORS");
	drawText(10, 10, "OPTION2: KEEP PLAYING");
 }
