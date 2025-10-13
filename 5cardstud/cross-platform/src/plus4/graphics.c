#ifdef __PLUS4__

#include <plus4.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include "charset.h"
#include "../platform-specific/graphics.h"

#define RVS_OFF 0x80
#define MULTICOLOR_MODE 0x10
#define COLS_40 0x08
#define SCREEN_LOC 0x0C00
#define DBLBUF_LOC 0x7400
#define SCREEN_COLORRAM_LOC 0x0800
#define DBLBUF_COLORRAM_LOC 0x7000
#define CHARSET_LOC 0xC000
#define SCR (unsigned char*)SCREEN_LOC
#define SCR_STATUS (SCR+40*23)
#define SCR_STATUS_DIRECT (unsigned char*)(DBLBUF_LOC+40*23)
#define SET_COL(val)  (*(unsigned char*) (0x53B) = (val));
#define COL_RED 8 + BCOLOR_RED | CATTR_LUMA3
#define COL_BLACK 8 + COLOR_BLACK
#define COL_WHITE 8 + COLOR_WHITE
#define COL_YELLOW 8 + COLOR_YELLOW
#define HIBASE 0x53E
#define KERNAL_CHAR_PTR 0xC8
#define KERNAL_CRAM_PTR 0xEA

bool always_render_full_cards=false;
unsigned char colorMode = 0;
static unsigned short screen = DBLBUF_LOC;
static unsigned char col_text = COL_BLACK;

// gotoxy + cput_ saves 4 bytes over cput_xy, so why not optimize?
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);

void enableDoubleBuffer()
{
    unsigned char page = DBLBUF_LOC >> 8;
    TED.video_addr = page;
    screen = DBLBUF_LOC;
}

void disableDoubleBuffer()
{
    unsigned char page = SCREEN_LOC >> 8;
    TED.video_addr = page;
    screen = SCREEN_LOC;
}

void drawBuffer()
{
    waitvsync();
    memcpy((unsigned char *)DBLBUF_COLORRAM_LOC,(unsigned char *)SCREEN_COLORRAM_LOC,2048);
}

void resetScreen()
{
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

void clearStatusBar()
{
    memset((unsigned char *)DBLBUF_LOC+920,0x80,80);
    memset((unsigned char *)SCREEN_LOC+920,0x80,80);
}

void drawStatusTextAt(unsigned char x, char* s)
{
    static unsigned char j,split;
    SET_COL(COL_BLACK);
    split = strlen(s)>40;

    while(j=*s++ & 0x7F) {


        // Translate lowercase from server
        if (j >= 97 && j <= 122) // 97-122 -32
            j-=32;

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

void drawStatusText(char* s)
{
    clearStatusBar();
    drawStatusTextAt(0, s);
}

void drawStatusTimer()
{
    POKE(screen+920+39,0x53);
}

void drawText(unsigned char x, unsigned char y, const char* s)
{
    unsigned char i,c;
    char tmp[128];

    strcpy(tmp,s);

    SET_COL(col_text);

    // Convert alpha to lowercase for custom font
    for(i=strlen(tmp)-1; i<255; i--)  {
        c=tmp[i];

        // Translate lowercase from server
        if (c >= 97 && c <= 122) // 97-122 -32
            tmp[i]-=32;
        // Translate uppercase from program
        else if (c>=193)
            tmp[i]-=128;
    }

    cputsxy(x,y,tmp);
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
    static unsigned char i;
    int suit, val, space, top, bottom;
    unsigned char col;
    unsigned char *loc;

    top=0x7776;
    space= isHidden ? 0x605F : 0x63A1;
    bottom=0x7978;

    // Card value (default with a blank 0x63 right space)
    val = 0x6300;
    i = s[0];

    // // Translate lowercase from server
    if (i >= 97 && i <= 122) // 97-122 -32
       i-=32;

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

    i = s[1];
    // // Translate lowercase from server
    if (i >= 97 && i <= 122) // 97-122 -32
       i-=32;

    // Card suit
    switch (i) {
      case 'h': suit= 0x6665; col = COL_RED; break;   // Heart
      case 'd': suit= 0x6867; col = COL_RED; break;   //Diamond
      case 'c': suit= 0x6A69; col = COL_BLACK; break; //Club
      case 's': suit= 0x6C6B; col = COL_BLACK; break; // Spade
      default: col = COL_BLACK;                    // Back of card
    }

    // Color the card
    loc = (unsigned char *)SCREEN_COLORRAM_LOC + y*40+x+40;
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

void drawChip(unsigned char x, unsigned char y)
{
    SET_COL(COL_RED);
    cputcxy(x,y,'J');
}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char o = y > 22 ? 0x80 : 0x00;
    unsigned char *loc = SCR + y*40+x;
    while(w--)
    {
        *loc++ = 0x41 + o;
    }
}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char o = y > 22 ? 0x80 : 0x00;
    unsigned char *loc = SCR + y*40+x;
    while(w--)
    {
        *loc++ = 0x00 + o;
    }
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
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

void drawBorder()
{
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

void drawLogo()
{
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

void initGraphics()
{
    // Enable multicolor mode
    TED.hscroll |= RVS_OFF | MULTICOLOR_MODE | COLS_40;
    TED.misc &= 0xFB; // pull from RAM by turning off bit 2.

    // Clear screen
    clrscr();
    memset((unsigned char *)DBLBUF_LOC,0x00,0x1000);

    enableDoubleBuffer();

    // Load and enable charset
    memcpy((unsigned char *)CHARSET_LOC,charset,2048);
    TED.char_addr = CHARSET_LOC >> 8;

    // Set color RAM to black with multicolor
    memset((unsigned char *)SCREEN_COLORRAM_LOC,0x08,1000);
    memset((unsigned char *)DBLBUF_COLORRAM_LOC,0x08,1000);

    // Set colors
    col_text = COL_WHITE;
    TED.bordercolor = BCOLOR_BLACK | CATTR_LUMA0;
    TED.bgcolor=BCOLOR_CYAN|CATTR_LUMA2;
    TED.color1=BCOLOR_BROWN|CATTR_LUMA5;
    TED.color2 = BCOLOR_WHITE | CATTR_LUMA7;

    clearStatusBar();
    drawStatusText("              PLUS/4 VERSION");
}

void resetGraphics()
{
}

uint8_t cycleNextColor()
{
    setColorMode((colorMode+1) % 8);
    return colorMode;
}

void setColorMode(unsigned char mode)
{
    colorMode = mode;
    waitvsync();

    switch(colorMode)
    {
    case 0:
        col_text = COL_WHITE;
        TED.bgcolor=BCOLOR_CYAN|CATTR_LUMA2;
        TED.color1=BCOLOR_BROWN|CATTR_LUMA5;
        break;
    case 1:
        col_text = COL_WHITE;
        TED.bgcolor=BCOLOR_BLUE|CATTR_LUMA2;
        TED.color1=BCOLOR_WHITE|CATTR_LUMA2;
        break;
    case 2:
        col_text = COL_WHITE;
        TED.bgcolor=BCOLOR_WHITE|CATTR_LUMA2;
        TED.color1=BCOLOR_WHITE|CATTR_LUMA5;
        break;
    case 3:
        col_text = COL_BLACK;
        TED.bgcolor=BCOLOR_ORANGE|CATTR_LUMA4;
        TED.color1=BCOLOR_BLUE|CATTR_LUMA3;
        break;
    case 4:
        col_text = COL_WHITE;
        TED.bgcolor=BCOLOR_PURPLE|CATTR_LUMA2;
        TED.color1=BCOLOR_LEMON|CATTR_LUMA5;
        break;
    case 5:
        col_text = COL_BLACK;
        TED.bgcolor=BCOLOR_BLUEGREEN|CATTR_LUMA4;
        TED.color1=BCOLOR_DARKBLUE|CATTR_LUMA3;
        break;
    case 6:
        col_text = COL_WHITE;
        TED.bgcolor=BCOLOR_BROWN|CATTR_LUMA1;
        TED.color1=BCOLOR_PURPLE|CATTR_LUMA4;
        break;
    case 7:
        col_text = COL_BLACK;
        TED.bgcolor=BCOLOR_GREEN|CATTR_LUMA4;
        TED.color1=BCOLOR_BLUE|CATTR_LUMA3;
        break;
    }
}

#endif /* __PLUS4__ */
