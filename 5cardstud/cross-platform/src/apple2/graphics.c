#ifdef __APPLE2__

/*
  Graphics functionality
*/


#include "hires.h"
#include "text.h"
#include<peekpoke.h>
#include<string.h>
#include "../platform-specific/graphics.h"


#define BOTTOM 175
#define RED_VAL_1 ROP_OR(0b11010110)
#define RED_VAL_2 ROP_OR(0b10101010)


unsigned char cycleNextColor() {
  return 0;
}

void enableDoubleBuffer() {
   POKE(0xc055,1);
}

void disableDoubleBuffer() {
  POKE(0xc054,1);
}

void clearStatusBar() {
  unsigned char i;
  
  for (i=0;i<40;i++) {
    hires_putc(i,175,ROP_AND(0x2A),' ');
    hires_putc(i,183,ROP_AND(0x2A),' ');
  }
}

void drawBuffer() {
   memcpy((void*)0x4000,(void*)0x2000,0x2000);
}

void drawStatusTextAt(unsigned char x, char* s) {
  unsigned char i,j;
  
  i=0;
  j=BOTTOM+8;
  
  while(*s){
    if (*s=='\n') {
      i=0;
      j+=8;
    } else
      hires_putc(i++,j,ROP_CPY,*s);
    s++;
  }
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
 
}

void drawText(unsigned char x, unsigned char y, char* s) {
  hires_puts(x,y*8-1,ROP_AND(0xFF), s);
}

// Call to clear the screen to an empty table
void resetScreen() { 
  static int i;
  // Clear screen except for corners
  
  for (i=0;i<39;++i) {
    hires_Mask(i,0,1,BOTTOM,0xa92A);
    hires_Mask(++i,0,1,BOTTOM,0xa955);
  }  

  // Draw rounded table corners
  hires_putc(0,0,ROP_AND(0x2A),0x01);
  hires_putc(39,0,ROP_AND(0x55),0x02);
  hires_putc(0,BOTTOM-8,ROP_AND(0x2A),0x03);
  hires_putc(39,BOTTOM-8,ROP_AND(0x55),0x04);

  // Reset screen to green background, black status bar, black char color
  clearStatusBar(); 
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s) {
  static unsigned char val, red, i;
  static unsigned suit, mid;
  mid = 0x0900;
  y*=8;
  y--;

  // Card value
  switch (s[0]) {
    case 't': val=0x71; break;
    case 'j': val=0x73; break;
    case 'q': val=0x75; break;
    case 'k': val=0x77; break;
    case 'a': val=0x79; break;
    case '?': val=0x7B; mid=0x7B7C; break;
    default:
      val=0x61 + 2*(s[0]-0x32);
  }

  // Card suit
  switch (s[1]) {
    case 'h' : suit=0x0A0B; red=1; break;
    case 'd' : suit=0x0C0D; red=1; break;
    case 'c' : suit=0x0E0F; red=0; break;
    case 's' : suit=0x1011; red=0; break;
    default: suit=0x7B7C; red=0; break;
  }

  // Card top
  hires_putcc(x,y,ROP_X,0x0506);

  // Card value
  hires_putc(x,y+=8, red ? RED_VAL_1 :ROP_CPY ,val);
  hires_putc(x+1,y,red? RED_VAL_2 : ROP_CPY,++val);

  // Card middle, suit, bottom
  hires_putcc(x,y+=8,ROP_CPY,mid);
  hires_putcc(x,y+=8,ROP_CPY,suit);
  hires_putcc(x,y+=8,ROP_X,0x0708);
}

void drawChip(unsigned char x, unsigned char y) {
  hires_putc(x,y*8-1,ROP_CPY,0x22);
}

void drawBlank(unsigned char x, unsigned char y) {
  hires_putc(x,y*8-1,ROP_AND(0x2A),0);
}

void drawPointer(unsigned char x, unsigned char y) {
 
}


void drawLine(unsigned char x, unsigned char y, unsigned char w) {
 
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
 
}

void drawBorder() {
  drawCard(1,0,FULL_CARD, "as");
  drawCard(37,0,FULL_CARD, "ah");
  drawCard(1,17,FULL_CARD, "ad");
  drawCard(37,17,FULL_CARD, "ac");
}

void drawLogo() {
  static unsigned char i;
  i=4;
  drawText(14,++i, "             ");
  drawText(14,++i, "  FUJI  NET  ");
  drawText(14,++i, "             ");
  drawText(14,++i, " 5 CARD STUD ");
  drawText(14,++i, "             ");
}


void resetGraphics() {
 
}

void initGraphics() {
  hires_Init();
  enableDoubleBuffer();
}

void waitvsync() {
  // Unsupported in Apple 2?
}

#endif /* __APPLE2__ */