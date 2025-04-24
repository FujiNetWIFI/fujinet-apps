#ifdef __ADAM__
#include "../platform-specific/graphics.h"
#include <video/tms99x8.h>
#include <conio.h>
#include <sys/ioctl.h>
#include <smartkeys.h>
#include "udgs.h"
#include "eos.h"

//defines for tile locations
// #define POINTER_TILE 216
// #define CHIP_TILE 1
// #define UL_CORNER_TILE 144
// #define HORIZONTAL_TILE 145
// #define UR_CORNER_TILE 146
// #define LL_CORNER_TILE 147
// #define LR_CORNER_TILE 148
// #define VERTICAL_TILE 149

#define POINTER_TILE 0xbe
#define CHIP_TILE 0xbc
#define UL_CORNER_TILE 0xb6
#define HORIZONTAL_TILE 0xb7
#define UR_CORNER_TILE 0xb8
#define LL_CORNER_TILE 0xb9
#define LR_CORNER_TILE 0xba
#define VERTICAL_TILE 0xbb

// tile data for the corners of the screen to round them off...
const unsigned char upperLeftCorner[] = { 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xff};
const unsigned char upperRightCorner[] = { 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xfe, 0xff, 0xff};
const unsigned char lowerLeftCorner[] = { 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x0f, 0x07};
const unsigned char lowerRightCorner[] = { 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};

// tiles for moves
const char sCall[] = "\x9a\x9b\x9c";
const char sRaise[] = "\x9d\x9e\x9f";
const char sBet[] = "\xa0\xa1";
const char sCheck[] = "\xa2\xa3\xa4";
const char sFold[] = "\xa5\xa6\xa7";

enum {SPADES=0,CLUBS,DIAMONDS,HEARTS};

unsigned char pointerLocation_x = 255;
unsigned char pointerLocation_y = 255;
extern unsigned long currentTime;
bool always_render_full_cards = 0;
bool vsync = false;


void enableDoubleBuffer()
{
}

void disableDoubleBuffer()
{
}

void drawBuffer()
{
}

// Call to clear the screen to an empty table
void resetScreen()
{
    vdp_vfill(0x8,0,240);
    vdp_vfill(0x0100,0,0x1600);
    vdp_vfill(0x1708,0,240);
    vdp_vfill(0x2100,0xfc,0x1600);
}

void clearStatusBar()
{
    vdp_vfill(0x1600,0,256);
    vdp_vfill(0x1708,0,240);
}

void drawStatusTextAt(unsigned char x, char* s)
{
    // textcolor(WHITE);
    // gotoxy(x,22);
    // cputs(s);
    drawText(x,22,s);
}

void drawStatusText(char* s)
{
    static char* comma;
    clearStatusBar();
    textcolor(WHITE);
    if (strlen(s)>62) {
        comma = s;
        while (*comma++!=',');
            comma[0]=0;
        comma++;
        // gotoxy(0,22);
        // cputs(s);
        // gotoxy(0,23);
        // cputs(comma);
        smartkeys_puts(8,176,s);
        smartkeys_puts(8,184,comma);

        // drawText(1, 22, s);
        // drawText(1, 23, comma);
    } else {
        smartkeys_puts(8,176,s);
        // drawStatusTextAt(1, s);
    }


}

void drawStatusTimer()
{
    // gotoxy(10,0);
    // printf("timer %d",currentTime/60);
}

void drawText(unsigned char x, unsigned char y, char* s)
{
    gotoxy(x,y);
    if (strcmp(s,"FOLD") == 0)
        cputs(sFold);
    else if (strcmp(s,"CALL") == 0)
        cputs(sCall);
    else if (strcmp(s,"CHECK") == 0)
        cputs(sCheck);
    else if (strcmp(s,"RAISE") == 0)
        cputs(sRaise);
    else if (strcmp(s,"BET") == 0)
        cputs(sBet);
    else 
    {
        textcolor(WHITE);
        textbackground(GREEN);
        gotoxy(x,y);
        cputs(s);
        // smartkeys_puts(x*8,y*8,s);
    }
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
    char red = 0;
    char suit;
    unsigned int checkscreenloc = 0;
    bool adjacentCard = false;
    static char testbytes[8];

    if (x+3 <= 32) checkscreenloc = (x*(y+1)*8) + 24;

    vdp_vread(checkscreenloc,testbytes,sizeof(testbytes));
    for (i = 0; i < sizeof(testbytes); i++)
    {
        if (testbytes[i] != 0 ) {// data in here...
            adjacentCard = true;
            break;
        }
    }

    textcolor(LIGHTBLUE);
    textbackground(GREEN);

    //cards in tile set are 5 tiles tall, and 2 tiles wide for a full card (3 at the end), 
    //and 1 tile wide for partial
    // x=x/2;
    gotoxy(x,y++);

    //draw the top of the card
    if (partial == PARTIAL_LEFT)
    {
        cputc(0x93);
        textbackground(WHITE);
        gotoxy(x,y++);
        cputc(0xb0);
        gotoxy(x,y++);
        cputc(0xb2);
        gotoxy(x,y++);
        cputc(0xb4);
        textbackground(GREEN);
        gotoxy(x,y++);
        cputc(0x94);
    } else if (partial == PARTIAL_RIGHT)
    {
        // x++;
        cputc(0x97);
        textbackground(WHITE);
        gotoxy(x,y++);
        cputc(0xad);
        gotoxy(x,y++);
        cputc(0xae);
        gotoxy(x,y++);
        cputc(0xaf);
        gotoxy(x,y++);
        textbackground(GREEN);
        cputc(0x98);
    } else {
        switch (toupper(s[1])) {
            case 'H' : suit=0x8f + HEARTS; red=RED; break;
            case 'D' : suit=0x8f + DIAMONDS; red=RED; break;
            case 'C' : suit=0x8f + CLUBS; red=BLACK; break;
            case 'S' : suit=0x8f + SPADES; red=BLACK; break;
            case '?' : red=LIGHTBLUE; break;
            default: suit=0; red=0; break;
        }
    
        cputs("\x93\x95\xa9");
        gotoxy(x,y++);
        textcolor(LIGHTBLUE);
        textbackground(WHITE);

        if (s[0] == '?')
            cputc(0xb0);
        else 
            cputc(0x99);

        // Card value row
        textcolor(red);
        switch (toupper(s[0])) {
            case 'T': cputs("\x8a"); break;
            case 'J': cputs("\x8b"); break;
            case 'Q': cputs("\x8c"); break;
            case 'K': cputs("\x8d"); break;
            case 'A': cputs("\x8e"); break;
            case '?': cputs("\xb1"); break;
            default:
                cputc(s[0] + 0x50);
        }

        textcolor(LIGHTBLUE);
        textbackground(GREEN);
        if (!adjacentCard) cputc(0x99);

        textcolor(LIGHTBLUE);
        textbackground(WHITE);

        // blank middle row of card
        gotoxy(x,y++);
        if (s[0] == '?') //back of card
        {
            cputs("\xb2\xb3"); textbackground(GREEN); if (!adjacentCard) cputs("\x99");
        }
        else
        {
            cputs("\x99\x80"); textbackground(GREEN); if (!adjacentCard) cputs("\x99");
        }


        // suit row of card .. depends on ? or not...
        gotoxy(x,y++);
        textbackground(WHITE);
        if (s[0] == '?')
        {
            cputs("\xb4\xb5");
        } else {
            cputs("\x99"); 
            textcolor(red);
            cputc(suit); 
        }

        textcolor(LIGHTBLUE); textbackground(GREEN);  if (!adjacentCard) cputs("\x99");

        textcolor(LIGHTBLUE);
        textbackground(GREEN);

        // bottom border of card
        gotoxy(x,y++);
        cputs("\x94\x96");  if (!adjacentCard) cputs("\xa8");
    }
}

void drawChip(unsigned char x, unsigned char y)
{
    textcolor(YELLOW);
    textbackground(GREEN);

    gotoxy(x,y);
    cputc(CHIP_TILE);
}

void drawPointer(unsigned char x, unsigned char y)
{
    textcolor(CYAN);
    textbackground(GREEN);
    gotoxy(x,y);
    cputc(POINTER_TILE);
}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char i;

    textcolor(RED);
    textbackground(GREEN);

    gotoxy(x,y);
    for (i=0; i<w; i++)
        cputc(HORIZONTAL_TILE);
}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char i;

    textcolor(WHITE);
    textbackground(GREEN);

    gotoxy(x,y);
    for (i=0; i<w; i++)
        cputc(0x20);
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    unsigned char i;

    textcolor(RED);
    textbackground(GREEN);

    gotoxy(x,y++);
    //Draw the top
    cputc(UL_CORNER_TILE);
    for (i = 1; i <= w; i++)
        cputc(HORIZONTAL_TILE);
    cputc(UR_CORNER_TILE);

    //Draw the sides
    for (i = 0; i < h; i++)
    {
        gotoxy(x,y);
        cputc(VERTICAL_TILE);
        gotoxy(x+w+1,y++);
        cputc(VERTICAL_TILE);
    }
    gotoxy(x,y);
    //Draw the bottom
    cputc(LL_CORNER_TILE);
    for (i = 1; i <= w; i++)
        cputc(HORIZONTAL_TILE);
    cputc(LR_CORNER_TILE);
}

void drawBorder()
{
}

void drawLogo()
{
    smartkeys_puts(80,88,"FIVE CARD STUD");
}

void setColorMode()
{
}

unsigned char cycleNextColor()
{
}


//This is a debug function to print out a set of cards on the display to make sure the correct tiles are used and they 
// display correctly
void testCardTiles()
{
    unsigned char card[2];
    int x; int y;

    resetScreen();


    card[0] = '?';
    card[1] = '?';
    drawCard(1, 1, FULL_CARD, card, false);

    card[0] = 'A';
    card[1] = 'D';
    drawCard(5, 1, FULL_CARD, card, false);

    card[0] = 'K';
    card[1] = 'C';
    drawCard(9, 1, FULL_CARD, card, false);

    card[0] = 'Q';
    card[1] = 'S';
    drawCard(13, 1, FULL_CARD, card, false);

    card[0] = 'J';
    card[1] = 'H';
    drawCard(17, 1, FULL_CARD, card, false);

    drawCard(20,1, PARTIAL_LEFT,card,false);
    drawCard(22,1, PARTIAL_RIGHT,card,false);

    card[0] = '9';
    card[1] = 'D';
    drawCard(24,1, PARTIAL_LEFT,card,false);
    drawCard(25,1, FULL_CARD,card,false);
    drawCard(27,1, PARTIAL_RIGHT,card,false);


    card[0] = '2';

    for (x = 0; x<6; x++)
    {
        drawCard((x*4) + 1, 6, FULL_CARD, card, false);
        card[0]++;
    }

    card[1] = 'S';
    for (x = 0; x<6; x++)
    {
        drawCard((x*4) + 1, 11, FULL_CARD, card, false);
        card[0]++;
    }


}

void myInt()
{
    currentTime++;
    vsync=true;
}

void initGraphics()
{
    //set up the vdp vram with tile data, both tile and color... change to mode 1 first thing.
    //change background to medium green...
	unsigned int reg = 0;

    smartkeys_set_mode();
    void *param = &udgs;
    console_ioctl(IOCTL_GENCON_SET_UDGS,&param);
    vdp_color(VDP_INK_WHITE,VDP_INK_DARK_GREEN,VDP_INK_BLACK);
    add_raster_int(myInt);

    clrscr();

    // put the corners on the table
    vdp_vwrite(upperLeftCorner,0, sizeof(upperLeftCorner));
    vdp_vfill(0x2000,0xc1,8);
    vdp_vwrite(upperRightCorner,0x00f8,sizeof(upperRightCorner));
    vdp_vfill(0x20f8,0xc1,8);
    vdp_vwrite(lowerLeftCorner,0x1700,sizeof(lowerLeftCorner));
    vdp_vfill(0x3700,0xc1,8);
    vdp_vwrite(lowerRightCorner,0x17f8,sizeof(lowerRightCorner));
    vdp_vfill(0x37f8,0xc1,8);

    // testCardTiles();
    // drawPointer(31,20);
    // while(1){};
}

void waitvsync()
{
    while(!vsync);
    vsync=false;
}

#endif /* __ADAM__ */
