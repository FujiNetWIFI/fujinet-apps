#ifdef _CMOC_VERSION_

/*
  Graphics functionality
*/
#include "coco_bool.h"

#include "hires.h"
#include "../platform-specific/graphics.h"
#include "../misc.h"

#define BOTTOM 175

#define USER_HAND_ROW       149
#define DISPLAYED_CARD_COL  14
#define DISPLAYED_CARD_ROW  48

#define CARDS_PER_ROW  15

#define OVERLAY_HEIGHT 30  /* A card on top of another one must be drawn this many pixels lower, so enough of the bottom card shows. */

#define STATUS_COL 6
#define STATUS_ROW 1  // in characters

#define DECK_COL 21  // in bytes of pixels
#define DECK_ROW 8  // in pixels

#define EVENT_ROW 13

#define REQUESTED_SUIT_COL 19  // in characters
#define REQUESTED_SUIT_ROW  9  // in characters

#define QUIT_GAME 0
#define NEXT_PLAYER_PLAYS 1
#define NEXT_PLAYER_PASSES 2
#define PLAY_NEW_GAME 3

#define RED 0b01010101
#define RED_RIGHT 0b01010100

extern unsigned char charset[];
extern unsigned int charset_len;

bool always_render_full_cards = 0;
uint8_t xor_mask=0;

unsigned char colorMode=0;

unsigned char cycleNextColor() {
  return 0;
}

void setColorMode(unsigned char mode) {
  colorMode = mode;
}

void enableDoubleBuffer() {
    // Not implemented
}

void disableDoubleBuffer() {
    // not implemented
}

void drawTextAt(unsigned char x, unsigned char y, const char *s) {
    static unsigned char c;

    while(*s) {
        c=*s++;
        if (c>=97 && c<=122) c=c-32;
        hires_putc(x++,y,0,c);
    }
}

void clearStatusBar() {
  hires_Mask(0,178,32,14,0);
  //unsigned char i;
  //hires_Mask(0,175,40,17,0xa900);
  //for (i=0;i<40;i++) {
   // hires_putc(i,175,0,' ');
   // hires_putc(i,183,0,' ');
 // }
}

void drawBuffer() {
  waitvsync();
}

void drawStatusTextAt(unsigned char x, const char* s) {
  drawTextAt(x, BOTTOM+6, s);
}

void drawStatusText(const char* s) {
  static char* comma;
  unsigned char len = strlen(s);
  //clearStatusBar();
  if (len>32) {
      comma = (char *)s;
    while (*comma++!=',');
    comma[0]=0;
    comma++;
    drawTextAt(0, BOTTOM-1, s);
    drawTextAt(0, BOTTOM+8, comma);
  } else {
    drawStatusTextAt(0, s);
    if (len<32)
      hires_Mask(len,178,32-len,14,0);
  }

}

void drawStatusTimer() {
  hires_putc(WIDTH-1,BOTTOM+5,0, 0x28);
}


void drawText(unsigned char x, unsigned char y, const char* s) {
  drawTextAt(x,y*8, s);
}


void drawChip(unsigned char x, unsigned char y) {
  hires_putc(x,y*8,0, 0x22);
}


// Call to clear the screen to an empty table
void resetScreen() {
     //pcls(0);
     hires_Mask(0,0,32,178,0);
}

void drawCardAt(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  static unsigned char val, red, redR, i, suit,rightDigit;
  static unsigned mid;
  uint8_t* pos;

  mid = 0x1213;

  if (partial == PARTIAL_LEFT) {
    hires_Draw(x,y+5,4,0,&charset[(uint16_t)(0x01<<3)+4]);
    hires_putc(x,y+=8,0 ,0x5f);
    hires_putc(x,y+=8,0 ,0x5f);
    hires_Draw(x,y+=8,6,0,&charset[(uint16_t)((0x5f)<<3)]);
    hires_Draw(x,y+=5,5,0,&charset[(uint16_t)0x03<<3]);
  } else if (partial == PARTIAL_RIGHT) {
    ++x;
    hires_Draw(x,y+5,4,0,&charset[(uint16_t)(0x02<<3)+4]);
    hires_putc(x,y+=8,0 ,0x1f);
    hires_putc(x,y+=8,0 ,0x1f);
    hires_Draw(x,y+=8,6,0,&charset[(uint16_t)((0x1F)<<3)]);
    hires_Draw(x,y+=5,5,0,&charset[(uint16_t)0x04<<3]);
  } else { // Full card
  
    switch (s[1]) {
      case 'h' : suit=0x0A; red=RED; redR=RED_RIGHT; break;
      case 'd' : suit=0x0C; red=RED; redR=RED_RIGHT; break;
      case 'c' : suit=0x0E; red=redR=0; break;
      case 's' : suit=0x10; red=redR=0; break;
      default: suit=0x7B; red=redR=0; break;
    }

    
    if (s[0]=='?') {
      // Overturned card

      // Top edge
      hires_Draw(x,y+5,4,0,&charset[(uint16_t)(0x01<<3)+5]);
      hires_Draw(x+1,y+5,4,0,&charset[(uint16_t)(0x02<<3)+5]);

      // Middle 
      hires_putcc(x,y+=8,0 ,0x1e1f);
      hires_putcc(x,y+=8,0 ,0x1e1f);
      hires_Draw(x,y+=8,7,0,&charset[(uint16_t)((0x1E)<<3)]);
      hires_Draw(x+1,y,7,0,&charset[(uint16_t)((0x1F)<<3)]);

      // Bottom edge
      hires_Draw(x,y+=6,4,0,&charset[(uint16_t)(0x03<<3)]);
      hires_Draw(x+1,y,4,0,&charset[(uint16_t)(0x04<<3)]);

      // Since a full overturned card is being drawn, we may have just folded.
      // Blank out the rest of the hand by it (since no double buffer is used to clear screen)
      if (x<20)
        hires_Mask(x+2,y-26,8,29,0);
      else
        hires_Mask(x-8,y-26,8,29,0);
    
    } else {

      rightDigit=0x13;
      switch (s[0]) {
        case 't': val=0x1c;rightDigit=0x1d; break;
        case 'j': val=0x23;  break;
        case 'q': val=0x24; break;
        case 'k': val=0x25;  break;
        case 'a': val=0x26; break;
        default:
          val=0x14+(s[0]-0x32);
      }
      
      // Card left edge    
      if (x>0) {
        pos = (uint8_t *)SCREEN+(uint16_t)(y+6)*32+x-1;

        // Make first downturned card slightly smaller
        if (x==29 && *(pos+2)) {
          drawCardAt(30,y, PARTIAL_RIGHT, "??", 0);
        }
        
        if (!*pos || *pos==0x80) {
          for(i=24;i<255;--i) {
            *(pos) += 02;
            pos+=32;
          }
        } else if (*(pos+64)==173) { //x==1 || (x==13 && y<100)) {
          //cgetc();
         // printf("%u",*(pos+64));
         // cgetc();
          // Make first downturned card slightly smaller
          drawCardAt(x-1,y, PARTIAL_LEFT, "??", 0);
          
        }
      }
      
      xor_mask = isHidden;
      // Top edge
      hires_Draw(x,y+5,4,0,&charset[(uint16_t)(0x05<<3)+5]);
      hires_Draw(x+1,y+5,4,0,&charset[(uint16_t)(0x06<<3)+5]);
      
      // Card value
      hires_putc(x,y+=8, red ,val);
      hires_putc(x+1,y,  redR,rightDigit);

      // Middle
      hires_Draw(x,y+=8,7,0,&charset[(uint16_t)((mid >> 8 & 0xFF)<<3)]);
      hires_Draw(x+1,y,7,0,&charset[(uint16_t)((mid & 0xFF)<<3)]);
      
      // Suit
      hires_putc(x,y+=6,0 ,suit);
      hires_putc(x+1,y, 0,++suit);
      
      // Bottom edge
      hires_Draw(x,y+=8,4,0,&charset[(uint16_t)0x07<<3]);
      hires_Draw(x+1,y,4,0,&charset[(uint16_t)0x08<<3]);
      xor_mask = 0;

      // If drawing the full hand on the left side, shift the rendered cards to the right one pixel
      // to make room to draw the left edge border on the first card.
      // This only occurs on final hand flips
      if (x==8) { 
        y+=2;
        for(i=0;i<28;i++) {
          pos = (uint8_t *)SCREEN+(uint16_t)y*32+10;
          for (x=0;x<11;x++) {
            if (x>0)
              *pos=*pos>>2;
            else 
              *pos=*pos&0x03 + (*pos>>2);
            if (x<10)
              *pos+=(*(pos-1)&0x03)<<6;
            else if (i>0 && i<27)
              *pos+=0x80;
            pos--;
          }
          y--;
        }
      }
    }

     
  }
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  y=y*8-2;
  drawCardAt(x, y, partial, s, isHidden);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  y=y*8;
  if (y > 181)
    y=189;
  hires_Mask(x,y,w,2, 0b11111111);
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
  y=y*8;
  if (y > 181)
    y=189;
   hires_Mask(x,y,w,2, 0);
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  y=y*8-1;

  // Top Corners
  hires_putc(x,y,0, 0x3b);hires_putc(x+w+1,y,0, 0x3c);

  // Accents if height > 1
  // if (h>1) {
  //   hires_putc(x+1,y+8,0, 1);
  // }

  // Top/bottom lines
  for(i=x+w;i>x;--i) {
    hires_putc(i,y,0, 0x40);
    hires_putc(i,y+(h+1)*8,0, 0x40);
  }

  // Sides
  for(i=0;i<h;++i) {
    y+=8;
    hires_putc(x,y,0, 0x3f);
    hires_putc(x+w+1,y,0, 0x3f);
  }

    // Accents if height > 1
  // if (h>1) {
  //   hires_putc(x+w,y,0, 4);
  // }

  y+=8;
  // Bottom Corners
  hires_putc(x,y,0, 0x3d);hires_putc(x+w+1,y,0, 0x3e);


}

void drawBorder() {
  static uint8_t i,y;
  static uint8_t* pos;

  //drawCardAt(12,20,FULL_CARD, "??", 0);
  //drawCardAt(13,20,FULL_CARD, "as", 0);

  // for(i=0;i<32;i+=11) {
  //   drawCardAt(i,20,FULL_CARD, "as", 0);
  //   drawCardAt(i+2,20,FULL_CARD, "as", 0);
  //   drawCardAt(i+4,20,FULL_CARD, "as", 0);
  //   drawCardAt(i+6,20,FULL_CARD, "as", 0);
  //   drawCardAt(i+8,20,FULL_CARD, "as", 0);    
  // }
  // cgetc();

  drawCardAt(1,-5,FULL_CARD, "as", 0);
  drawCardAt(29,-5,FULL_CARD, "ah", 0);
  drawCardAt(1,152-8,FULL_CARD, "ad", 0);
  drawCardAt(29,152-8,FULL_CARD, "ac", 0);

  for(i=4;i<28;i+=4) {
    hires_putcc(i,2,0, 0x7475);
    hires_putcc(i+2,2,0, 0x7677);
    hires_putcc(i,168,0, 0x7475);
    hires_putcc(i+2,168,0, 0x7677);
  } 

  // drawCardAt(5,60,FULL_CARD, "??", 0);
  
  // drawCardAt(10,60,PARTIAL_LEFT, "??", 0);
  // drawCardAt(11,60,FULL_CARD, "as", 0);

  // drawCardAt(15,60,PARTIAL_RIGHT, "??", 0);
  // drawCardAt(14,60,FULL_CARD, "as", 0);
  
}

void drawLogo() {
  static unsigned char i;
  i=4;
  drawText(11,++i, "           ");
  drawText(11,++i, " FUJI  NET ");
  drawText(11,++i, "           ");
  drawText(11,++i, "5 CARD STUD");
  drawText(11,++i, "           ");
}


void resetGraphics() {

}

void initGraphics() {
    // int i;
    // char c,b,v;
    // initCoCoSupport();

  //pmode(4,SCREEN); pcls(0); screen(1,1); 

  pmode(3,SCREEN); pcls(0); screen(1,0); 

  //*((uint8_t*)0xFFB0)=30; // Green
  *((uint8_t*)0xFFB1)=63;// White
  *((uint8_t*)0xFFB2)=0;// White
  *((uint8_t*)0xFFB3)=22;// Red
    
}

#endif
