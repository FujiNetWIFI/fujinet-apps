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

#ifdef COCO3 
#define CORNER_TOP 0
#define CORNER_BOTTOM 140
#define STATUS_BLANK 0b10101010
#define COCO3_ADJUST_Y y++;
#else
#define CORNER_TOP -5
#define CORNER_BOTTOM 144 
#define STATUS_BLANK 0
#define COCO3_ADJUST_Y
#endif
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

extern uint8_t charset[];
uint8_t paletteBackup[8];

bool always_render_full_cards = 0;
uint8_t xor_mask=0;
uint8_t font_shift=0;

// Currently this is only for CoCo3
unsigned char colorMode=0;

void updateColors() {
    if (colorMode==1)
        setRGB();
    else
        setComposite();
}

unsigned char cycleNextColor() {
    ++colorMode;
    if (colorMode>2)
        colorMode=1;
    updateColors();
    return colorMode;
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
#ifdef COCO3
    if (y<180)
        y++;
#endif
    while(*s) {
        c=*s++;
        if (c>=97) c-=32;
        hires_putc(x++,y,0,c);
    }
}

void clearStatusBar() {
  hires_Mask(0,179,WIDTH,13,STATUS_BLANK);
}

void drawBuffer() {
  waitvsync();
}

void drawStatusTextAt(unsigned char x, const char* s) {
  font_shift=1;
  drawTextAt(x, BOTTOM+6, s);
  font_shift=0;
}

void drawStatusText(const char* s) {
  static char* comma;
  
  unsigned char len = strlen(s);
  if (len>WIDTH) {
      comma = (char *)s;
    while (*comma++!=',');
    comma[0]=0;
    comma++;
    clearStatusBar();
    font_shift=1;
    drawTextAt(0, BOTTOM-1, s);
    drawTextAt(0, BOTTOM+8, comma);
    font_shift=0;
  } else {
    drawStatusTextAt(0, s);
    if (len<WIDTH)
      hires_Mask(len,179,WIDTH-len,13,STATUS_BLANK);
  }

}

void drawStatusTimer() {
#ifdef COCO3
  font_shift=1;
#endif
  
  hires_putc(WIDTH-1,BOTTOM+5,0, 0x28);
#ifdef COCO3
  font_shift=0;
#endif
}


void drawText(unsigned char x, unsigned char y, const char* s) {
  drawTextAt(x,y*8, s);
}


void drawChip(unsigned char x, unsigned char y) {
#ifdef COCO3
    hires_putc(x,y*8+1,0, 0x22);
#else
    hires_putc(x,y*8,0, 0x22);
#endif
  
}


// Call to clear the screen to an empty table
void resetScreen() {
    uint8_t i;
    
#ifdef COCO3
    
    // Clear top and bottom 
    memset((uint8_t *)SCREEN, 0, WIDTH * 3);
    memset((uint8_t *)SCREEN+WIDTH*176, 0, WIDTH * 3);

    SCREEN[0]         = 0b10100000;
    SCREEN[WIDTH*1]   = 0b10000000;
    SCREEN[WIDTH*2]   = 0b10000000;
    SCREEN[WIDTH*178] = 0b10100000;
    SCREEN[WIDTH*177] = 0b10000000;
    SCREEN[WIDTH*176] = 0b10000000;
    
    SCREEN[39+0]         = 0b00001010;
    SCREEN[39+WIDTH*1]   = 0b00000010;
    SCREEN[39+WIDTH*2]   = 0b00000010;
    SCREEN[39+WIDTH*178] = 0b00001010;
    SCREEN[39+WIDTH*177] = 0b00000010;
    SCREEN[39+WIDTH*176] = 0b00000010;
    memset((uint8_t *)SCREEN+WIDTH*3, 0, WIDTH * 173);
#else
    memset((uint8_t *)SCREEN, 0, WIDTH * 179);
#endif
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
      // Draw full card
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
        pos = (uint8_t *)SCREEN+(uint16_t)(y+6)*WIDTH+x-1;

        // Handle special cases for right side of screen
        if (x==WIDTH-3) {
             // for 32 WIDTH, make first downturned card slightly smaller
            if (WIDTH == 32 && *(pos+2)) {
                drawCardAt(WIDTH-2,y, PARTIAL_RIGHT, "??", 0);
            } else if (WIDTH==40) {
                // At this position in WIDTH 40, means drawing first card at the end of game. Clear the hidden card to the right of it
                hires_Mask(x+1,y+1,1,28,0);
            }
        }

        // Draw card left edge
        if (!*pos || *pos==0x80) {
          for(i=24;i<255;--i) {
            *(pos) += 02;
            pos+=WIDTH;
          }
        }      
        else if (*(pos+WIDTH*2)==173) { 
          // Make first downturned card slightly smaller for left side
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
#ifndef COCO3
      if (x==8) {
        y+=2;
        for(i=0;i<28;i++) {
          pos = (uint8_t *)SCREEN+(uint16_t)y*WIDTH+10;
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
#endif
    }
  }
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, bool isHidden) {
  y=y*8-2;
  COCO3_ADJUST_Y
  drawCardAt(x, y, partial, s, isHidden);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w) {
  y=y*8;
  COCO3_ADJUST_Y
  
  if (y > 181)
    y=189;
  hires_Mask(x,y,w,2, 0b11111111);
}

void hideLine(unsigned char x, unsigned char y, unsigned char w) {
 uint8_t color=0;

  y=y*8;
  COCO3_ADJUST_Y

  if (y > 181) {
    y=189;
    #ifdef COCO3
      color=STATUS_BLANK;
    #endif
  }
   hires_Mask(x,y,w,2, color);
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

  drawCardAt(1,CORNER_TOP,FULL_CARD, "as", 0);
  drawCardAt(WIDTH-3,CORNER_TOP,FULL_CARD, "ah", 0);
  drawCardAt(1,CORNER_BOTTOM,FULL_CARD, "ad", 0);
  drawCardAt(WIDTH-3,CORNER_BOTTOM,FULL_CARD, "ac", 0);

  for(i=4;i<WIDTH-4;i+=4) {
    hires_putcc(i,CORNER_TOP+6,0, 0x7475);
    hires_putcc(i+2,CORNER_TOP+6,0, 0x7677);
    hires_putcc(i,CORNER_BOTTOM+24,0, 0x7475);
    hires_putcc(i+2,CORNER_BOTTOM+24,0, 0x7677);
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
  drawText(WIDTH/2-5,++i, "           ");
  drawText(WIDTH/2-5,++i, " FUJI  NET ");
  drawText(WIDTH/2-5,++i, "           ");
  drawText(WIDTH/2-5,++i, "5 CARD STUD");
  drawText(WIDTH/2-5,++i, "           ");
}


void resetGraphics() {
  waitvsync();

#ifdef COCO3
  // Restore palette/screen width
  memcpy((byte *) 0xFFB0, &paletteBackup, 8);
  width(32);
#endif

  pmode(0, 0x400);
  screen(0,0);
}


void setRGB()
{
#ifdef COCO3
    rgb();
    
    paletteRGB(0,0,2,1);   // Background
    paletteRGB(1,3,3,3);   // card bkg
    paletteRGB(2,0,0,0);   // border/text
    paletteRGB(3,2,0,0);
#endif
}

void setComposite()
{
#ifdef COCO3
    cmp();
    palette(0,30);  // Background
    palette(1,63);  // card background - white
    palette(2,0);   // black
    palette(3,6);  // red
#endif
}

void rgbOrComposite() {
#ifdef COCO3
    if (!isCoCo3)
        return; // not a coco3, we can't change palettes anyway.
    
    // Backup palette
    memcpy(&paletteBackup, (byte *) 0xFFB0, 8);

    asm { sync } // wait for v-sync to change the graphics mode
    
    // Allow border color by switching to CoCo 3 graphics mode verison of PMODE 3:
    *(byte *)0xFF90 = 0x4C;           // reset CoCo 2 compatible bit
    *(byte *)0xFF98 = 0x80;           // graphics mode
    *(byte *)0xFF9A = 0;   // make border black
    
    // GIME graphics mode register bits
    // .XX..... : Scan Lines : 0=192, 1=200, 3=225
    // ...XXX.. : Bytes/row  : 0=16, 1=20, 2=32, 3=40, 4=64, 5=80, 6=128, 7=160
    // ......XX : Pixels/byte: 0=8 (2 color), 1=4 (4 color), 2=2 (16 colors)
    *(byte *)0xFF99 = 0b00001101;     // 160x192x4 colors - 40x24 characters
    
    // Tell GIME the location of the screen, 0x6000, which by default is mapped to 0x3B by MMU.
    *(uint16_t *)0xFF9D = 0x3B << 10;
    
    if (colorMode==0) {
        drawTextAt(8,96,"R-GB or C-OMPOSITE");

        while (!colorMode)
        {
            switch(cgetc())
            {
            case 'R':
            case 'r':
                colorMode = 1;
                break;
            case 'C':
            case 'c':
                colorMode = 2;
                break;
            }
        }
    }

     updateColors();
#endif
}

void initGraphics() {

    initCoCoSupport();
    
    pmode(3,SCREEN);
    pcls(0);
    screen(1,0);
    
    clearStatusBar();
    resetScreen();

    rgbOrComposite();

    // Correct the status timer character for CoCo3
#ifdef COCO3
    charset[(uint16_t)(0x28<<3)+2]=charset[(uint16_t)(0x28<<3)+3]=0b11101010;
#endif
}

#endif
