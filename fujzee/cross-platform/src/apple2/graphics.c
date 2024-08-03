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
#define RED_VAL_1 ROP_OR(0b11010100)
#define RED_VAL_2 ROP_OR(0b10101010)
//#define RED_VAL_2 ROP_OR(0b11010101)
//#define RED_VAL_1 ROP_OR(0b10101010)

unsigned char colorMode=0;

unsigned char cycleNextColor() {
  ++colorMode;
  if (colorMode>1)
    colorMode=0;
  return colorMode;
}

void setColorMode(unsigned char mode) {
  colorMode = mode;
}

void enableDoubleBuffer() {
  POKE(0xc055,1);
}

void disableDoubleBuffer() {
 POKE(0xc054,1);
}

void drawTextAt(unsigned char x, unsigned char y, char*s) {
  static unsigned char c;
  while(*s) {
    c=*s++;
    if (c>=97 && c<=122) c=c-32;
    hires_putc(x++,y,ROP_CPY,c);
  }  
}

void clearStatusBar() {
  //unsigned char i;
  hires_Mask(0,175,40,17,0xa900);
  //for (i=0;i<40;i++) {
   // hires_putc(i,175,ROP_CPY,' ');
   // hires_putc(i,183,ROP_CPY,' ');
 // }
}

void drawBuffer() {
   memcpy((void*)0x4000,(void*)0x2000,0x2000);
}

void drawStatusTextAt(unsigned char x, char* s) {
  drawTextAt(x, BOTTOM+5, s);
}

void drawStatusText(char* s) {
  static char* comma;
  clearStatusBar();

  drawTextAt(0, BOTTOM-9,"CLYD BOT'S MOVE");
  drawTextAt(0, BOTTOM+9,"+WHA!");

  drawTextAt(18, BOTTOM-9," ERIC A2:NOT AGAIN");
  drawTextAt(18,BOTTOM,   "    THOM:SIGH");
  drawTextAt(18,BOTTOM+9, "MOZZWALD:I HATE CLYD");
//NNNNNNNN'S MOVE
//>12345678901234  NNNNNNNN:12345678901234
                        //WAITING ON XXXXXXXX
return;

  if (strlen(s)>40) {
    comma = s;
    while (*comma++!=',');
    comma[0]=0;
    comma++;
    drawTextAt(0, BOTTOM-1, s);
    drawTextAt(0, BOTTOM+8, comma);
  } else {
    drawStatusTextAt(0, s);
  }
  
}

void drawStatusTimer() {
  hires_putcc(38,BOTTOM+5,ROP_CPY, 0x2829);
}
  

void drawText(unsigned char x, unsigned char y, char* s) {
  drawTextAt(x,y*8-4, s);

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
    y=y*8-4;
    while (*s) {
      hires_Mask(x++,y,1,8,0xa955 - (0x2B*(x%2))); 
      s++;
    }
  }
  else*/

  //  hires_puts(x,y*8-4, s);
}


void drawChip(unsigned char x, unsigned char y) {
  //hires_putc(x-1,y*8-4,0xa955 - (0x2B*(x%2)), '1');
  //hires_putc(x,y*8-4,ROP_OR(0x55), 'O');
  // Solve gfx later once layout is finalized
  //hires_putc(x,y*8-4,ROP_OR(0x55), 'O');
  hires_putc(x,y*8-3,ROP_CPY, 0x22);
  
}


// Call to clear the screen to an empty table
void resetScreen() { 
  hires_Mask(0,1,39,174,0xa900);
  /*hires_Mask(0,2,1,1, 0xa9ff); 
  hires_Mask(1,1,38,1, 0xa9ff); 
  hires_Mask(39,2,1,1, 0xa9ff); 

  hires_Mask(0,2,1,1, 0xa9ff); 
  hires_Mask(1,1,38,1, 0xa9ff); 
  hires_Mask(39,2,1,1, 0xa9ff); */
  
  // Top corners
  hires_putc(0,0,ROP_CPY,0x01);
  hires_putc(39,0,ROP_CPY,0x02);
  
  // Top horizontal
  hires_Mask(1,1,38,1, 0xa9ff); 
  
  // Bottom corners
  hires_putc(0,BOTTOM-16,ROP_CPY,0x03);
  hires_putc(39,BOTTOM-16,ROP_CPY,0x04);

  // Bottom hor
  hires_Mask(1,BOTTOM-10,38,1, 0xa9ff); 
  
  // Sides
  hires_Mask(0,8,1,BOTTOM-24,ROP_CONST(0b00000110));
  hires_Mask(39,8,1,BOTTOM-24,ROP_CONST(0b00110000));

  //memset((void*)0x2000,0,0x2000);

  // Starting with black background first to "get it working", and many have monochrome monitors
  /*static int i;
  // Clear screen except for corners
  
  for (i=0;i<39;++i) {
    hires_Mask(i,0,1,BOTTOM,0xa92A);
    hires_Mask(++i,0,1,BOTTOM,0xa955);
  }  
*/
  // Draw rounded table corners
  // hires_putc(0,0,ROP_AND(0x2A),0x01);
  // hires_putc(39,0,ROP_AND(0x55),0x02);
  // hires_putc(0,BOTTOM-8,ROP_AND(0x2A),0x03);
  // hires_putc(39,BOTTOM-8,ROP_AND(0x55),0x04);
  
  // Reset screen to green background, black status bar, black char color
  //clearStatusBar(); 
}

void drawCardAt(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  static unsigned char val, red, i, suit;
  static unsigned mid;
  mid = isHidden ? 0x7D7E : 0x0900;
  //mid = 0x0900;

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
    case 'h' : suit=0x0A; red=1; break;
    case 'd' : suit=0x0C; red=1; break;
    case 'c' : suit=0x0E; red=0; break;
    case 's' : suit=0x10; red=0; break;
    default: suit=0x7B; red=0; break;
  }
  
  // Card top
  //hires_putcc(x,y,ROP_O,0x0506); 
  hires_Mask(x,y+7,1,1, ROP_CONST(0b01111100)); 
  hires_Mask(x+1,y+7,1,1, ROP_CONST(0b00111111)); 

  // Card value
  hires_putc(x,y+=8, colorMode && red ? RED_VAL_1 :ROP_CPY ,val);
  hires_putc(x+1,y, colorMode && red ? RED_VAL_2 : ROP_CPY,++val);
  
  // Card middle
  hires_putcc(x,y+=8,ROP_CPY,mid);
 
  // Suit
  hires_putc(x,y+=8,colorMode && red ? RED_VAL_1 :ROP_CPY ,suit);
  hires_putc(x+1,y,colorMode && red ? RED_VAL_2 : ROP_CPY,++suit);

  // Card bottom
  //hires_putcc(x,y+=8,ROP_CPY,0x0708); 
  hires_Mask(x,y+=8,1,1, ROP_CONST(0b01111100)); 
  hires_Mask(x+1,y,1,1, ROP_CONST(0b00111111)); 

 
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  y*=8;
  y-=7;
  drawCardAt(x, y, partial, s, isHidden);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
   hires_Mask(x,y*8-3,w,2, 0xa9ff); 
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
   hires_Mask(x,y*8-3,w,2, 0xa900); 
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  y=y*8-4;

  // Top Corners
  hires_putc(x,y,ROP_CPY, 0x3b);hires_putc(x+w+1,y,ROP_CPY, 0x3c);
  
  // Accents if height > 1
  if (h>1) {
    hires_putc(x+1,y+8,ROP_CPY, 1);
  }

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
  
    // Accents if height > 1
  if (h>1) {
    hires_putc(x+w,y,ROP_CPY, 4);
  }

  y+=8;
  // Bottom Corners
  hires_putc(x,y,ROP_CPY, 0x3d);hires_putc(x+w+1,y,ROP_CPY, 0x3e);
  

}

void drawBorder() {
  drawCardAt(1,0,FULL_CARD, "ah", 0);
  //drawCardAt(4,0,FULL_CARD, "th", 0);
  drawCardAt(37,0,FULL_CARD, "ah", 0);
  drawCardAt(1,126,FULL_CARD, "ad", 0);
  drawCardAt(37,126,FULL_CARD, "ac", 0);
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
  //uint16_t c=0x900;
  //bool use=false;
  hires_Init();
  
  // while(++c<0xD00) {
  //   //if (c%8 ==0)
  //    // use=!use;
  //   //if (use)
  //     POKE(c, (PEEK(c)&0b01111111)>>1+128);
  // }
}

void waitvsync() {
  static uint16_t i;
  // Aproximate a jiffy for the timer countdown
  for ( i=0;i<630;i++);
}

#endif /* __APPLE2__ */