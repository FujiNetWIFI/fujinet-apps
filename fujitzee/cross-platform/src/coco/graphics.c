#ifdef _CMOC_VERSION_

/*
  Graphics functionality
*/

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"

#include "hires.h"
#include "text.h"
#include "../platform-specific/graphics.h"
#include "../misc.h"

#define BOTTOM 175
#define RED_VAL_1 ROP_OR(0b11010100)
#define RED_VAL_2 ROP_OR(0b10101010)

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
  //  POKE(0xc055,1);
}

void disableDoubleBuffer() {
  // POKE(0xc054,1);
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
  //hires_Mask(0,175,40,17,0xa900);
  //for (i=0;i<40;i++) {
   // hires_putc(i,175,ROP_CPY,' ');
   // hires_putc(i,183,ROP_CPY,' ');
 // }
}

void drawBuffer() {
  //   memcpy((void*)0x4000,(void*)0x2000,0x2000);
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

void drawTextAlt(unsigned char x, unsigned char y, char* s) {
  static unsigned char c;
  static unsigned char* pos;

//  pos = xypos(x,y);
//
//  while(c=*s++) {
//    if (c<65 && c>=32) c-=32;
//    if (c<65 || c> 90)
//      c+=128;
//    else      
//      c+=32;
//
//    *pos++ = c;
//  }  
}

void drawTextVert(unsigned char x, unsigned char y, char* s) {
  static unsigned char c;
  static unsigned char* pos;

//  pos = xypos(x,y);
//
//  while(c=*s++) {
//    if (c<65 && c>=32) c-=32;
//    *pos = c;
//    pos+=40;
//  }  
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
  pcls(0);

#if 0
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
#endif
}

void drawDie(unsigned char x, unsigned char y, unsigned char s, bool isSelected) {
  static unsigned char *source, *dest;
  
// source=diceChars + (s-1)*9 ; // Locate the diceChar index for this die number
// 
// // Change the dice color
// if (isSelected)
//   source+=54;
//
// dest = xypos(x,y);
// 
// memcpy(dest, source, 3);
// memcpy(dest+40, source+3, 3);
// memcpy(dest+80, source+6, 3);
}

void drawClock(unsigned char x, unsigned char y) {
  //  POKE(xypos(x,y),0x37);
}

void drawBlank(unsigned char x, unsigned char y) {
  // POKE(xypos(x,y),0);
}

void drawSpace(unsigned char x, unsigned char y, unsigned char w) {
  // memset(xypos(x,y),0,w);
}

void drawTextCursor(unsigned char x, unsigned char y) {
  // POKE(xypos(x,y),0xD9);
}

void drawScoreCursor(unsigned char x, unsigned char y) {
  // POKE(xypos(x,y),0xBE);
}

/// @brief Returns true if the screen location is empty
bool isEmpty(unsigned char x, unsigned char y) {
  //return PEEK(xypos(x,y))==0;
}


void drawBoard() {
  static uint8_t y,x,c;
  static unsigned char *dest, *value;

  resetScreen();

//  // Thin horz ines
//  memset(xypos(0,7),84,40);
//  memset(xypos(11,10),84,29);
//  memset(xypos(0,20),84,10);
//
//  // Thick horz lines
//  memset(xypos(11,18),82,29);
//  memset(xypos(11,20),82,29);

  // Main scores box
  //  drawBox(10,0,5,19);

  // Vertical lines
//  c=91;
//  for (y=0;y<21;y++) {
//    dest = xypos(20,y);
//    for (x=0;x<5;x++) {
//      *dest=c;
//      dest+=4;
//    }
//    c=124;
//    if (y==19)
//      c = 88;
//  }
//
//  // Cross areas
//  for (x=10;x<40;x+=4) {
//    POKE(xypos(x,7),83);
//    POKE(xypos(x,10),83);
//    POKE(xypos(x,18),86);
//    if (x==10)
//      x+=2;
//  }
//
//  POKE(xypos(16,20),88);
//
//  // Score names
//  value = strtok(scores, ",");
//  for(y = 0; y<16; y++) {
//      drawTextAlt(11,scoreY[y],scores[y]);
//  }
//  
//  // Fujitzee score!
//  memcpy(xypos(11,17),&"89:;<",5);

}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  //   hires_Mask(x,y*8-3,w,2, 0xa9ff); 
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
  //   hires_Mask(x,y*8-3,w,2, 0xa900); 
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

void drawCursor(unsigned char x, unsigned char y) {
  static unsigned char i;
  static unsigned char* pos;

//  pos = xypos(x-1,y-1);
//
//  // Top row
//  *pos=177;
//  memset(pos+1,182,3);
//  *(pos+4)=178;
//
//  // Sides
//  for(i=0;i<3;i++) {
//    pos+=40;
//    *pos=*(pos+4)=181;
//  }
//  
//  // Bottom row
//  *(pos+=40)=179;
//  memset(pos+1,182,3);
//  *(pos+4)=180;
}

void hideCursor(unsigned char x, unsigned char y) {
static unsigned char i;
  static unsigned char* pos;

//  pos = xypos(x-1,y-1);
//
//  // Top row
//  memset(pos,0,5);
//
//  // Sides
//  for(i=0;i<3;i++) {
//    pos+=40;
//    *pos=*(pos+4)=0;
//  }
//  
//  // Bottom row
//  memset(pos+40,0,5);
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
