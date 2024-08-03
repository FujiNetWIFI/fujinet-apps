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

#define CHARSET_LOC 0xC000
#define DBLBUF_LOC 0xC800
#define SCREEN_LOC 0xCC00

// Multi Colors
#define COL_RED 8 + COLOR_RED
#define COL_BLACK 8 + COLOR_BLACK
#define COL_WHITE 8 + COLOR_WHITE
#define COL_YELLOW 8 + COLOR_YELLOW

#define COL_TEXT COL_BLACK // white may not be enough contrast?

// Double buffer memory location
#define SCR (unsigned char*)DBLBUF_LOC
#define SCR_STATUS (SCR+40*24)

#define SET_COL(val)  (*(unsigned char*) (646) = (val));
#define SET_VIC_BITMAP(_addr) VIC.addr = (VIC.addr & 0b11110001) | ((((_addr)>>8)&0x38)>>2);
#define SET_VIC_SCREEN(_addr) VIC.addr = (VIC.addr & 0b00001111) | ((((_addr)>>8)&0x3c)<<2);
#define SET_VIC_BANK(_addr) CIA2.pra = (CIA2.pra & ~3) | (((((_addr)>>8)&0xc0)>>6)^3);

// gotoxy + cput_ saves 4 bytes over cput_xy, so why not optimize?
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);

unsigned char cycleNextColor() {
  return 0;
}

void enableDoubleBuffer() {
  SET_VIC_SCREEN(SCREEN_LOC);

  // Point the Kernal to the double buffered screen for cputs()
  POKE (648, 0xC8);
}

void disableDoubleBuffer() {
  waitvsync();
  // Point the Kernal to the screen for direct cputs()
  POKE (648, 0xCC);
}

void clearStatusBar() {
  memset(SCR_STATUS,0x80,40);
}

void drawBuffer() {
  waitvsync();
  memcpy((void*)SCREEN_LOC,(void*)DBLBUF_LOC,1024);
}

void drawStatusTextAt(unsigned char x, char* s) {
  static unsigned char i,j;
  SET_COL(COL_BLACK);
  
  for(i=strlen(s)-1; i<255; i--) {
    j=s[i];

    POKE(SCR_STATUS+x+i,j+ (j<0x40 ? j==0x20 ? 0x60 : 0x80 : j>0xC0 ? -0x40: 0x40) );
  }
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
  POKE(SCR_STATUS+39,0x53);
}

void drawText(unsigned char x, unsigned char y, char* s) {
  unsigned char i;
  SET_COL(COL_TEXT);

  // Convert alpha to lowercase for custom font
  for(i=strlen(s)-1; i<255; i--) 
    s[i]&=0x7F; // Remove the 8th bit
  
  cputsxy(x,y,s);
}

// Call to clear the screen to an empty table
void resetScreen() { 
  // Reset screen to green background, black status bar, black char color
  clearStatusBar(); 

  // Clear screen except for corners
  memset(SCR+1,0,38);
  memset(SCR+40,0,40*22);
  memset(SCR+40*23+1,0,38);

  // Draw rounded table corners
  POKE(SCR+0, 86);
  POKE(SCR+39, 87);
  POKE(SCR+40*23, 88);
  POKE(SCR+40*24-1, 85);
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s) {
    static unsigned char i;
    int suit, val, space, top, bottom;
    unsigned char col;
    unsigned char * loc;
    
    top=0x7776;
    space=0x63A1;
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
      POKE(loc+40, col);
      POKE(loc+80, col);
    }
    if (partial & 2) {
      POKE(loc+1, col);
      POKE(loc+41, col);
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
  SET_COL(COL_BLACK);
  cputcxy(x,y, 'A');
  while(--w)
    cputc('A');
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  static unsigned char i;
  SET_COL(COL_YELLOW);

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
    cputsxy(4+i,22, "UV");
    
    SET_COL(COL_BLACK);
    cputsxy(6+i,1, "WX");
    cputsxy(6+i,22, "WX");
  }

  drawCard(1,0,FULL_CARD, "as");
  drawCard(37,0,FULL_CARD, "ah");
  drawCard(1,19,FULL_CARD, "ad");
  drawCard(37,19,FULL_CARD, "ac");
}

void drawLogo() {
  static unsigned char i;
  i=4;
  SET_COL(COL_TEXT);
  cputsxy(18,i, "FEF");
  cputsxy(13,++i, "fuji");
  
  SET_COL(COL_BLACK);
  cputs("HEJEB");

  SET_COL(COL_TEXT);
  cputs("net");
  cputsxy(18,++i, "FFEB");
  cputsxy(19,++i, "GFF");

  cputsxy(17,i+2, "5 card stud");
}


void resetGraphics() {
  
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

  // Clear screen memory
  memset((void*)SCREEN_LOC,0,1000);

  // Load custom charset (and reset the screen) at the 4th bank (0xC000)
  memcpy((void*)CHARSET_LOC,&charset,2048);
  SET_VIC_BITMAP(CHARSET_LOC);
  SET_VIC_BANK(CHARSET_LOC);
  
  

  // Set color ram to black
  memset(COLOR_RAM,8,1000);

   // Set colors
  VIC.bordercolor=COLOR_BLACK;  // D020 / border
  VIC.bgcolor0=COLOR_GREEN;     // D021 / backgr
  VIC.bgcolor1=COLOR_BLUE;      // D022 / Multicolor 1
  VIC.bgcolor2=COLOR_WHITE;     // D023 / Multicolor 2
}


#endif /* __C64__ */