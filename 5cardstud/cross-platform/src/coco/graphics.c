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

#define RED 0b10101010

extern unsigned char charset[];
extern unsigned int charset_len;

char blah = 'c';
bool always_render_full_cards = 1;

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
  drawTextAt(x, BOTTOM+5, s);
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
  hires_putcc(WIDTH-2,BOTTOM+5,0, 0x2829);
}


void drawText(unsigned char x, unsigned char y, const char* s) {
  drawTextAt(x,y*8-4, s);
}


void drawChip(unsigned char x, unsigned char y) {
  hires_putc(x,y*8-3,0, 0x22);
}


// Call to clear the screen to an empty table
void resetScreen() {
     //pcls(0);
     hires_Mask(0,1,32,177,0);
}

void drawCardAt(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  static unsigned char val, red, i, suit;
  static unsigned mid;
  static unsigned char cardgameVal, cardgameSuit;
  mid = isHidden ? 0x7D7E : 0x0900;

  switch (s[1]) {
    case 'h' : suit=0x0A; red=1; cardgameSuit=0; break;
    case 'd' : suit=0x0C; red=1; cardgameSuit=2; break;
    case 'c' : suit=0x0E; red=0; cardgameSuit=3; break;
    case 's' : suit=0x10; red=0; cardgameSuit=1; break;
    default: suit=0x7B; red=0; cardgameSuit=4; break;
  }

  // Card value
  switch (s[0]) {
    case 't': val=0x71; cardgameVal=10; break;
    case 'j': val=0x73; cardgameVal=11;  break;
    case 'q': val=0x75; cardgameVal=12;  break;
    case 'k': val=0x77; cardgameVal=13;  break;
    case 'a': val=0x79; cardgameVal=1;  break;
    case '?': val=0x7B; cardgameVal=((s[1]=='h')||(s[1]=='h'))?1:2; mid=0x7B7C; break;
    default:
      val=0x61 + 2*(s[0]-0x32);
      cardgameVal=(s[0]-0x32);
  }

  // Card top (draw bottom 2 rows)
  //hires_putcc(x,y,0,0x0506);
  hires_Draw(x,y+6,3,0,&charset[(uint16_t)(0x05<<3)+6]);
  hires_Draw(x+1,y+6,3,0,&charset[(uint16_t)(0x06<<3)+6]);

  // Card value
  hires_putc(x,y+=8,  red ? RED :0 ,val);
  hires_putc(x+1,y,  red ? RED : 0,++val);

  // Card middle
  hires_putcc(x,y+=8,0,mid);

  // Suit
  hires_putc(x,y+=8,0 ,suit);
  hires_putc(x+1,y, 0,++suit);

  // Card bottom (draw top two rows)
  //override_height=2;
  //hires_putcc(x,y+=8,0,0x0708);
  hires_Draw(x,y+=8,3,0,&charset[(uint16_t)0x07<<3]);
  hires_Draw(x+1,y,3,0,&charset[(uint16_t)0x08<<3]);
  //override_height=8;
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  y*=8;
  y-=7;
  drawCardAt(x, y, partial, s, isHidden);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  y=y*8-3;
  if (y == 181)
    y=189;
  hires_Mask(x,y,w,2, 0b01010101);
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
  y=y*8-3;
  if (y == 181)
    y=189;
   hires_Mask(x,y,w,2, 0);
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h) {
  y=y*8-4;

  // Top Corners
  hires_putc(x,y,0, 0x3b);hires_putc(x+w+1,y,0, 0x3c);

  // Accents if height > 1
  if (h>1) {
    hires_putc(x+1,y+8,0, 1);
  }

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
  if (h>1) {
    hires_putc(x+w,y,0, 4);
  }

  y+=8;
  // Bottom Corners
  hires_putc(x,y,0, 0x3d);hires_putc(x+w+1,y,0, 0x3e);


}

void drawBorder() {
  drawCardAt(1,0,FULL_CARD, "as", 0);
  drawCardAt(29,0,FULL_CARD, "ah", 0);
  drawCardAt(1,152-8,FULL_CARD, "ad", 0);
  drawCardAt(29,152-8,FULL_CARD, "ac", 0);
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
    int i;
    char c,b,v;
    initCoCoSupport();
    // for(i=0;i<charset_len;i++) {
    //   c = charset[i];
    //   if (c>0) {
    //     for(b=0;b<4;b++) {

    //       v=(c>>(b*2))&3;
    //       c=c-(v<<(b*2));
    //       switch(v) {
    //         case 0: c=c+(2<<(b*2)); break;
    //         //case 1: c=c+(3<<(b*2)); break;
    //         case 2: c=c+(3<<(b*2)); break;
    //         case 3: c=c+(1<<(b*2)); break;
    //       }
    //     }
    //     charset[i] = c;
    //   }
    // }

    pmode(4,SCREEN); // 3 for green
    pcls(0);
    screen(1,1); // 1,0 for green
}

#endif
