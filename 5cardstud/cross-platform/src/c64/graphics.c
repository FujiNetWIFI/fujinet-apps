#ifdef __C64__

/*
  Graphics functionality
*/


#include <conio.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "charset.h"
#include "../platform-specific/graphics.h"

// #define CHARSET_LOC 0xC000
// #define DBLBUF_LOC 0xC800
// #define SCREEN_LOC 0xCC00
// #define SPRITE_LOC 0xE000

// Bank 2
#define CHARSET_LOC 0x8000
#define DBLBUF_LOC 0x8800
#define SCREEN_LOC 0xA000
#define SPRITE_LOC 0xB000

// Multi Colors
#define COL_RED 8 + COLOR_RED
#define COL_BLACK 8 + COLOR_BLACK
#define COL_WHITE 8 + COLOR_WHITE
#define COL_YELLOW 8 + COLOR_YELLOW


// Double buffer memory location
#define SCR (unsigned char*)DBLBUF_LOC
#define SCR_STATUS (SCR+40*23)
#define SCR_STATUS_DIRECT (unsigned char*)(SCREEN_LOC+40*23)

#define SET_COL(val)  (*(unsigned char*) (646) = (val));
#define SET_VIC_BITMAP(_addr) VIC.addr = (VIC.addr & 0b11110001) | ((((_addr)>>8)&0x38)>>2);
#define SET_VIC_SCREEN(_addr) VIC.addr = (VIC.addr & 0b00001111) | ((((_addr)>>8)&0x3c)<<2);
#define SET_VIC_BANK(_addr) CIA2.pra = (CIA2.pra & ~3) | (((((_addr)>>8)&0xc0)>>6)^3);

// gotoxy + cput_ saves 4 bytes over cput_xy, so why not optimize?
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);

static unsigned short screen = SCREEN_LOC;
static unsigned char col_text = COL_BLACK;

bool always_render_full_cards = 0;

unsigned char colorMode=0;
//static unsigned char doubleBuf=0;

unsigned char cycleNextColor() {  
  setColorMode((colorMode+1 ) % 3);
  return colorMode;
}

void setColorMode(unsigned char mode) {
  colorMode = mode;
  waitvsync();
  if (colorMode == 0) {
    col_text = COL_BLACK;
    VIC.bgcolor0=COLOR_GREEN;     // D021 / backgr
    VIC.bgcolor1=COLOR_BLUE;      // D022 / Multicolor 1
  } else if (colorMode == 1) {
    col_text = COL_WHITE;
    VIC.bgcolor0=COLOR_BLUE;     // D021 / backgr
    VIC.bgcolor1=COLOR_GRAY2;      // D022 / Multicolor 1
  } else {
    col_text = COL_WHITE;
    VIC.bgcolor0=COLOR_GRAY1;     // D021 / backgr
    VIC.bgcolor1=COLOR_GRAY3;      // D022 / Multicolor 1
  }
}

void enableDoubleBuffer() {
  // Set visible screen
  SET_VIC_SCREEN(SCREEN_LOC);

  // Set location to write to 
  screen = DBLBUF_LOC;
  
  
  // Point the Kernal to the double buffered screen for cputs()
  waitvsync();
  POKE (648, DBLBUF_LOC/0x100);
  //drawBuffer();
}

void disableDoubleBuffer() {
  // Set visible screen to double buffered location
  SET_VIC_SCREEN(DBLBUF_LOC);
  screen = SCREEN_LOC;
  
  
  //waitvsync();
  // Point the Kernal to the screen for direct cputs()
  //POKE (648, 0xCC);
}

void clearStatusBar() {
  memset(screen+920,0x80,80);
}

void drawBuffer() {
  waitvsync();
  memcpy((void*)SCREEN_LOC,(void*)DBLBUF_LOC,1024);
}

void drawStatusTextAt(unsigned char x, char* s) {
  static unsigned char j,split;
  SET_COL(COL_BLACK);
  split = strlen(s)>40;

  while(j=*s++ & 0x7F) {
    POKE(SCR_STATUS+x,j+ (j<0x40 ? j==0x20 ? 0x60 : 0x80 : j>0xC0 ? -0x40: 0x40) );
    POKE(SCR_STATUS_DIRECT+x,j+ (j<0x40 ? j==0x20 ? 0x60 : 0x80 : j>0xC0 ? -0x40: 0x40) );
    ++x;

    // Split to second line on the first comma
    if (split && j==',' && x<40) {
       if (*s==' ') {
        ++s;
       }
      x=40;
    }
  }
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
  POKE(screen+920+39,0x53);
}

void drawText(unsigned char x, unsigned char y, const char* s) {
  unsigned char i;
  char tmp[128];

  strcpy(tmp,s);
  
  SET_COL(col_text);

  // Convert alpha to lowercase for custom font
  for(i=strlen(tmp)-1; i<255; i--) 
    if (tmp[i]>128) tmp[i]-=127;
  
  cputsxy(x,y,tmp);
}

// Call to clear the screen to an empty table
void resetScreen() { 
  
  // Clear screen except for corners
  memset(SCR+1,0,38);
  memset(SCR+40,0,40*21);
  memset(SCR+40*22+1,0,38);

  // Draw rounded table corners
  POKE(SCR+0, 86);
  POKE(SCR+39, 87);
  POKE(SCR+40*22, 88);
  POKE(SCR+40*23-1, 85);
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden) {
    static unsigned char i;
    int suit, val, space, top, bottom;
    unsigned char col;
    unsigned char * loc;
    
    top=0x7776;
    space= isHidden ? 0x605F : 0x63A1;
    bottom=0x7978;
     
    // Card value (default with a blank 0x63 right space)
    val = 0x6300;
    i = s[0];
    switch (i) {
      case 't': val=0xABAA; break; // 10 - unlike others, takes up two chars
      case 'j': val+=0x9c; break; // Jack
      case 'q': val+=0x9d; break; // Queen
      case 'k': val+=0x9e; break; // King
      case 'a': val+=0x9f; break; // Ace
      case 63 : // Card back
        suit=space=val=0x6261;  
        top+=0x404;
        bottom+=0x404;
      break;
      
      default:
        val += i + 0x70;  // 2 - 9
    }

    // Card suit
    switch (s[1]) {
      case 'h': suit= 0x6665; col = COL_RED; break;   // Heart
      case 'd': suit= 0x6867; col = COL_RED; break;   //Diamond
      case 'c': suit= 0x6A69; col = COL_BLACK; break; //Club
      case 's': suit= 0x6C6B; col = COL_BLACK; break; // Spade
      default: col = COL_BLACK;                    // Back of card
    }

    // Color the card
    loc = COLOR_RAM + y*40+x+40;
    if (partial & 1) {
      POKE(loc, col);
      POKE(loc+40, COL_BLACK);
      POKE(loc+80, col);
    }
    if (partial & 2) {
      POKE(loc+1, col);
      POKE(loc+41, COL_BLACK);
      POKE(loc+81, col);
    }
    
    // Draw card
    loc = SCR + y*40+x;

    // Top of card
    POKEW(loc, top);loc+=39;

    // Draw the left border char only if not overlaying an existing card
    if (!PEEK(loc)) {
      POKE(loc, 0x6e); 
      POKE(loc+40, 0x6e); 
      POKE(loc+80, 0x6e); 
    }

    // Draw the value, white space, suite, and bottom
    POKEW(++loc, val);  
    POKEW(loc+=40, space);      
    POKEW(loc+=40, suit);     
    POKEW(loc+=40, bottom);
}

void drawChip(unsigned char x, unsigned char y) {
  SET_COL(COL_RED);
  cputcxy(x,y,'J');
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  static uint16_t xx;
  //xx=x;
  // if (y<23)  {
  //   SET_COL(w<9 ? COL_YELLOW : COL_BLACK);
  //   cputcxy(x,y, 'A');
  //   while(--w)
  //     cputc('A');
  // } else { 
  //   SET_COL(COL_BLACK);
  //   j='-';
  //   w++;
  //   while(--w) {
  //     POKE(SCREEN_LOC+x+w+y*40-1,j+ (j<0x40 ? j==0x20 ? 0x60 : 0x80 : j>0xC0 ? -0x40: 0x40) );
  //     POKE(DBLBUF_LOC+x+w+y*40-1,j+ (j<0x40 ? j==0x20 ? 0x60 : 0x80 : j>0xC0 ? -0x40: 0x40) );
  //   }
    
  // }

  if (w>8 && y<23) {
    SET_COL(col_text);
    cputcxy(x,y, 'A');
    while(--w)
      cputc('A');
  } else {
    // Update sprite
    xx=(x+3)*8;
    VIC.spr_pos[0].x = xx;
    VIC.spr_hi_x = xx>256;
    VIC.spr_pos[0].y = VIC.spr_pos[1].y = y*8+52;
    xx+=48;
    VIC.spr_pos[1].x = xx;
    VIC.spr_hi_x = VIC.spr_hi_x|((xx>256)<<1);
     
    memset((char*)SPRITE_LOC, 0, 68);
    
    for (x=0;x<w && x<6;x++) {
      y=x % 2 ? 0xFF : 0xF0;
      POKE(SPRITE_LOC+(x>>1), y);
      POKE(SPRITE_LOC+3+(x>>1), y);
    }

    POKE(SPRITE_LOC+64, w==7 ? 0xF0 : w==8 ? 0xFE : 0);
    POKE(SPRITE_LOC+67, w==7 ? 0xF0 : w==8 ? 0xFE : 0);

   
   // for(x=0;x<8;x++) {
   
   //   xx++;
   //   waitvsync();
   // }
  }

}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
  memset((char*)SPRITE_LOC, 0, 68);
 

  // if (y<23) {
  //   SET_COL(COL_BLACK);
  //   cputcxy(x,y, ' ');
  //   while(--w)
  //     cputc(' ');
  // } else {
  //   memset(SCREEN_LOC+x+y*40,0x80,w);
  //   memset(DBLBUF_LOC+x+y*40,0x80,w);
  // }
  //  SET_COL(COL_YELLOW);
  //  cputcxy(x,y, ' ');
  //   while(--w)
  //   cputc(' ');

   //hires_Mask(x,y*8-3,w,2, 0xa900); 
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  static unsigned char i;
  SET_COL(COL_YELLOW);
  --w;
  // Top row
  cputcxy(x,y, ';');
  for(i=w;i<255;i--)
    cputc('A');
  cputc('<');

  // Sides
  for(i=y+h;i>y;i--) {
    cputcxy(x,i, '?');
    cputcxy(x+w+2,i, '?');
  }

  // Bottom
  i=y+h+1;
  cputcxy(x, i, '=');
  for(i=w;i<255;i--)
    cputc('A');
  cputc('>');
}

void drawBorder() {
  static unsigned char i;
  for(i=0;i<32;i+=4) { 
    SET_COL(COL_RED);
    cputsxy(4+i,1, "UV");
    cputsxy(4+i,21, "UV");
    
    SET_COL(COL_BLACK);
    cputsxy(6+i,1, "WX");
    cputsxy(6+i,21, "WX");
  }

  drawCard(1,0,FULL_CARD, "as", 0);
  drawCard(37,0,FULL_CARD, "ah", 0);
  drawCard(1,18,FULL_CARD, "ad", 0);
  drawCard(37,18,FULL_CARD, "ac", 0);
}

void drawLogo() {
  static unsigned char i;
  i=4;
  SET_COL(col_text);
  cputsxy(18,i, "FEF");
  cputsxy(13,++i, "fuji");
  
  SET_COL(COL_BLACK);
  cputs("HEJEB");

  SET_COL(col_text);
  cputs("net");
  cputsxy(18,++i, "FFEB");
  cputsxy(19,++i, "GFF");

  cputsxy(17,i+2, "5 card stud");
}


void resetGraphics() {
  VIC.spr_exp_x=VIC.spr_ena=0;

  waitvsync();
  
  // Return screen/bank to initial values
  CIA2.pra = 0xC7;
  VIC.ctrl2 = 200;
  VIC.addr = 0x17;
  POKE (648, 0x04);

  // Reset colors
  memset(COLOR_RAM, 0xE,1000);
  POKE (646, 0x0E);
  VIC.bordercolor=0xfe;
  VIC.bgcolor0=0xf6;
  VIC.bgcolor1=0xf1;
  VIC.bgcolor2=0xf2;

  POKE(0x291,0); //re-enable SHIFT-Commodore changing of char sets
}

void initGraphics() {
  POKE(0x291,0x80); //disable SHIFT-Commodore changing of char sets
  
  //set multicolor mode
  VIC.ctrl2 = 0xD8;  

  // Clear screen
  memset((void*)DBLBUF_LOC,0,1000);
  memset(DBLBUF_LOC+920,0x80,80);

  // Load custom charset (and reset the screen) at the 4th bank (0xC000)
  memcpy((void*)CHARSET_LOC,&charset,2048);
  SET_VIC_BITMAP(CHARSET_LOC);
  SET_VIC_BANK(CHARSET_LOC);

  // Set color ram to black
  memset(COLOR_RAM,8,1000);

  enableDoubleBuffer();
  

   // Set colors
  VIC.bordercolor=COLOR_BLACK;  // D020 / border
  VIC.bgcolor0=COLOR_GREEN;     // D021 / backgr
  VIC.bgcolor1=COLOR_BLUE;      // D022 / Multicolor 1
  VIC.bgcolor2=COLOR_WHITE;     // D023 / Multicolor 2

  // Setup cursor sprite
  

  
  for(i=0;i<2;i++) {
    POKE(DBLBUF_LOC + 0x3f8+i, (SPRITE_LOC-CHARSET_LOC) / 64+i);
    memset((char*)SPRITE_LOC+i*64, 0, 63);
    VIC.spr_color[i] = COLOR_YELLOW;
  }

  VIC.spr_exp_x=0b00000011;

  // enable sprites
  VIC.spr_ena = 0b00000011;


  

}


#endif /* __C64__ */
