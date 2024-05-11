#ifdef __APPLE2__

/*
  Graphics functionality
*/


#include "hires.h"
#include "text.h"
#include<peekpoke.h>
#include<string.h>
#include "../platform-specific/graphics.h"
#include "../misc.h"

#define BOTTOM 175
#define RED_VAL_1 ROP_OR(0b11010110)
#define RED_VAL_2 ROP_OR(0b10101010)

unsigned char colorMode=1;

void cycleNextColor() {
  ++colorMode;
  if (colorMode>1)
    colorMode=0;
}

void enableDoubleBuffer() {
  POKE(0xc055,1);
}

void disableDoubleBuffer() {
 POKE(0xc054,1);
}

void clearStatusBar() {
  //unsigned char i;
  hires_Mask(0,183,40,8,0xa900);
  //for (i=0;i<40;i++) {
   // hires_putc(i,175,ROP_CPY,' ');
   // hires_putc(i,183,ROP_CPY,' ');
 // }
}

void drawBuffer() {
   memcpy((void*)0x4000,(void*)0x2000,0x2000);
}

void drawStatusTextAt(unsigned char x, char* s) {
  //hires_puts(x, BOTTOM+8, s);
  drawText(x, BOTTOM+8, s);

  /*unsigned char i,j;
  
  i=x;
  j=BOTTOM+8;
  
  while(*s){
    if (*s=='\n') {
      i=0;
      j+=8;
    } else
      hires_putcU(i++,j,ROP_CPY,*s);
    s++;
  }*/
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
 
}
  
void drawText(unsigned char x, unsigned char y, char* s) {
  static unsigned char c;
  while(*s) {
    c=*s++;
    if (c>=97 && c<=122) c=c-32;
    hires_putc(x++,y*8-1,ROP_CPY,c);
  }   

  // Convert lowercase to upper
  //char*s2 = s;
  //unsigned char isSpace=1;

 // Display small space runs as blank table color
  /*while (*s2) {
   // if (*s2>=0x61 && *s2<=0x7a)
     // *s2-=0x20;
    if (*s2!=0x20)
      isSpace=0;
    s2++;
  }
  */
  
  /*if (isSpace && strlen(s) < 6) {
    y=y*8-1;
    while (*s) {
      hires_Mask(x++,y,1,8,0xa955 - (0x2B*(x%2))); 
      s++;
    }
  }
  else*/

  //  hires_puts(x,y*8-1, s);
}


void drawChip(unsigned char x, unsigned char y) {
  //hires_putc(x-1,y*8-1,0xa955 - (0x2B*(x%2)), '1');
  //hires_putc(x,y*8-1,ROP_OR(0x55), 'O');
  // Solve gfx later once layout is finalized
  //hires_putc(x,y*8-1,ROP_OR(0x55), 'O');
  hires_putc(x,y*8,ROP_CPY, 0x22);
  
}


// Call to clear the screen to an empty table
void resetScreen() { 
  hires_Mask(0,1,39,BOTTOM,0xa900);
  //memset((void*)0x2000,0,0x2000);

  // Starting with black background first to "get it working", and many have monochrome monitors
  /*static int i;
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
  */
  // Reset screen to green background, black status bar, black char color
  //clearStatusBar(); 
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
  hires_putcc(x,y,ROP_CPY,0x0506); 

  // Card value
  hires_putc(x,y+=8, colorMode && red ? RED_VAL_1 :ROP_CPY ,val);
  hires_putc(x+1,y, colorMode && red ? RED_VAL_2 : ROP_CPY,++val);
  
  // Card middle, suit, bottom
  hires_putcc(x,y+=8,ROP_CPY,mid);
  hires_putcc(x,y+=8,ROP_CPY,suit);
  hires_putcc(x,y+=8,ROP_CPY,0x0708); 
}

void drawPointer(unsigned char x, unsigned char y) {
 
}


void drawLine(unsigned char x, unsigned char y, unsigned char w) {
 
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  y=y*8-1;

  // Top Corners
  hires_putc(x,y,ROP_CPY, 0x3b);hires_putc(x+w+1,y,ROP_CPY, 0x3c);
  
  // Top/bottom lines
  for(i=x+w;i>x;--i) {
    hires_putc(i,y,ROP_CPY, 0x40);
    hires_putc(i,y+(h+1)*8,ROP_CPY, 0x40);
  }
  
  // Sides
  for(i=0;i<h;++i) {
    y+=8;
    hires_putc(x,y,ROP_CPY, 0x3f);
    hires_putc(x+w+1,y,ROP_CPY, 0x3f);
  }
  
  y+=8;
  // Bottom Corners
  hires_putc(x,y,ROP_CPY, 0x3d);hires_putc(x+w+1,y,ROP_CPY, 0x3e);
  
}

void drawBorder() {
  drawCard(1,0,FULL_CARD, "as");
  drawCard(37,0,FULL_CARD, "ah");
  drawCard(1,18,FULL_CARD, "ad");
  drawCard(37,18,FULL_CARD, "ac");
}

void drawLogo() {
  static unsigned char i;
  i=4;
  drawText(15,++i, "           ");
  drawText(15,++i, " FUJI  NET ");
  drawText(15,++i, "           ");
  drawText(15,++i, "5 CARD STUD");
  drawText(15,++i, "           ");
}


void resetGraphics() {
 
}

void initGraphics() {
  hires_Init();
  enableDoubleBuffer();
}

void waitvsync() {
  static uint16_t i;
  // Aproximate a jiffy for the timer countdown
  for ( i=0;i<630;i++);
}

#endif /* __APPLE2__ */