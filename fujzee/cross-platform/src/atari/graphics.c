#ifdef __ATARI__

/*
  Graphics functionality
*/


#include <conio.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <atari.h>

#include "charset.h"
#include "../platform-specific/graphics.h"


#define SCREEN_LOC 0xA000
#define CHARSET_LOC 0xA800
#define DISPLAY_LIST 0xB000

unsigned char colorMode=0;

// 26 lines
void DisplayList =
{
    DL_BLK8,DL_BLK8, // 2 Blanks Lines
    DL_LMS(DL_CHR40x8x4),SCREEN_LOC, // 1 Line
    DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4, // 5 Lines
    DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4, // 5 Lines
    DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4, // 5 Lines
    DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4, // 5 Lines
    DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4,DL_CHR40x8x4, // 5 Lines
    DL_JVB, DISPLAY_LIST
};

// Color scheme: Shadow, Highlights, White, Alt White, Background
const unsigned char colors[] = {
 0x00, 0x2A, 0x0E, 0xCC,  0x84, // NTSC
 0x00, 0xFA, 0x0E, 0xBC,  0x74 // PAL
};

unsigned char cycleNextColor() {
  return 0;
}

void setColorMode(unsigned char mode) {
 memcpy(&OS.color0, &colors, 5);
}


void initGraphics() {
  resetScreen();

  // Set Display list
  memcpy(DISPLAY_LIST,&DisplayList,sizeof(DisplayList));
  OS.sdlst = DISPLAY_LIST;

  // Load custom charset
  memcpy((void*)CHARSET_LOC,&charset,1024);
  OS.chbas = CHARSET_LOC/256;

  // Stub
  setColorMode(0);
}

void enableDoubleBuffer() {
}

void disableDoubleBuffer() {
  waitvsync();
}

void clearStatusBar() {

}

void drawBuffer() {
 // waitvsync();
 // memcpy((void*)SCREEN_LOC,(void*)DBLBUF_LOC,1024);
}

void drawStatusTextAt(unsigned char x, char* s) {
 drawTextAlt(x,HEIGHT-1,s);
}

void drawStatusText(char* s) {
  clearStatusBar();
  drawStatusTextAt(0, s);
}

void drawStatusTimer() {
  //POKE(SCR_STATUS+39,0x53);
}

void drawText(unsigned char x, unsigned char y, char* s) {
  static unsigned char c;
  static unsigned char* position;

  position = SCREEN_LOC + x+ y*40;

  while(*s) {
    c=*s++;
    if (c>=32 && c<65) c-=32;
    *position++ = c;
  }  
}


void drawTextAlt(unsigned char x, unsigned char y, char* s) {
  static unsigned char c;
  static unsigned char* pos;

  pos = SCREEN_LOC + x+ y*40;

  while(*s) {
    c=*s++;
    if (c>=32 && c<65) c-=32;
    if (c<65 || c> 90)
      c+=128;
    else      
      c+=32;

    *pos++ = c;
  }  
}


void resetScreen() { 
  waitvsync();
  // Clear screen memory
  memset((void*)SCREEN_LOC,0,26*40);
}

void drawDie(unsigned char x, unsigned char y, const char* s, bool isSelected) {
  static unsigned char c;
  static unsigned char* pos;
  
  c=s[0]-48; // Translate char to number 1-6
  pos = SCREEN_LOC + x+ y*40;
  
  *pos++ = c<4 ? 65 : 70;   // Top left
  *pos++ =64;               // Top center
  *pos = c<2 ? 66 : 71;     // Top right

  pos+=38;
  *pos++ = c<6 ? 64 : 74;  // Middle left
  *pos++ = c&1 ? 69 : 64;  // Middle center
  *pos = c<6 ? 64 : 75;    // Middle right
  
  pos+=38;
  *pos++ = c<2 ? 67 : 72;   // Bottom left
  *pos++ =64;               // Bottom center
  *pos = c<4 ? 68 : 73;     // Bottom right
}

void drawChip(unsigned char x, unsigned char y) {
  POKE(SCREEN_LOC+x+y*40,0x21);
}

void drawTextCursor(unsigned char x, unsigned char y) {
  POKE(SCREEN_LOC+x+y*40,0xD9);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  memset(SCREEN_LOC+x+y*40,82,w);
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
 
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  static unsigned char i;
  static unsigned char* pos;

  pos = SCREEN_LOC + x+ y*40;

  // Top row
  *pos=81;
  memset(pos+1,82,w);
  *(pos+w+1)=85;

  for(i=0;i<h;i++) {
    pos+=40;
    *pos=124;
    *(pos+w+1)=124;
  }

  
  // Bottom row
  pos+=40;
  *pos=87;
  memset(pos+1,82,w);
  *(pos+w+1)=90;

}

void resetGraphics() {
  
}



#endif /* __C64__ */