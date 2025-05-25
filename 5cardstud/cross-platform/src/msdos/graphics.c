#ifdef __WATCOMC__

#include "../platform-specific/graphics.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <dos.h>
#include "charset.h"

#define VIDEO_RAM_ADDR ((unsigned char far *)0xB8000000UL)
#define VIDEO_LINE_BYTES 80
#define VIDEO_ODD_OFFSET 0x2000

bool always_render_full_cards = 1;

unsigned char colorMode=0;

/**
 * @brief plot a 8x8 2bpp tile to screen at column x, row y
 * @param tile ptr to 2bpp tile data * 8
 * @param x Column 0-39
 * @param y Row 0-24
 */
void plot_tile(const unsigned char *tile, unsigned char x, unsigned char y)
{
    unsigned char far *video = VIDEO_RAM_ADDR;
    unsigned char i=0;

    y <<= 3; // Convert row to line
    x <<= 1; // Convert column to video ram offset

    for (i=0;i<8;i++)
    {
        unsigned char r = y + i;
        unsigned char rh = r >> 1; // Because CGA is interleaved odd/even.
        unsigned short ro = rh * VIDEO_LINE_BYTES + x;

        // If row is odd, go into second bank.
        if (r & 1)
            ro += VIDEO_ODD_OFFSET;

        // Put tile data into video RAM.
        video[ro] = tile[i*2];
        video[ro+1] = tile[i*2+1];
    }
}

/**
 * @brief Clear screen to given color index
 * @param c Color to set in all spots
 */
void clear(unsigned char c)
{
    unsigned char b = ((c & 0x03) | ((c & 0x03) << 2) | ((c & 0x03) << 4) | ((c & 0x03) << 6));
    unsigned char far *video = VIDEO_RAM_ADDR;

    _fmemset(&video[0x0000], b, 8000);
    _fmemset(&video[0x2000], b, 8000);
}

/**
 * @brief Text output
 * @param x Column
 * @param y Row
 * @param s Text to output
 */
void drawText(unsigned char x, unsigned char y, const char *s)
{
    signed char c=0;

    while (c = *s++)
    {
        c -= 32;
        if (c<0 || c>96)
            continue;

        if (x>39)
        {
            x=0;
            y++;
        }

        plot_tile(&ascii[c],x++,y);
    }
}

void enableDoubleBuffer()
{
    // Not implemented
}

void disableDoubleBuffer()
{
    // Not implemented
}

void drawBuffer()
{
    // Not implemented
}

// Call to clear the screen to an empty table
void resetScreen()
{
    clear(1); // Draw table color across whole screen
    plot_tile(&border[0],0,0);
    plot_tile(&border[1],39,0);
    plot_tile(&border[2],0,24);
    plot_tile(&border[3],39,24);
}

void clearStatusBar()
{
    /* unsigned char far *video = VIDEO_RAM_ADDR; */
    /* _fmemset(&video[0x1CC0],0x00,640); */
    /* _fmemset(&video[0x1CC0+0x2000],0x00,640); */
}

void drawStatusTextAt(unsigned char x, char* s)
{
    clearStatusBar();
    drawText(x,23,s);
}

void drawStatusText(char* s)
{
    clearStatusBar();
    drawText(0,23,s);
}

void drawStatusTimer()
{
}

/**
 * @brief is a card present? check for 0x55 (color 1)
 * @param x X coordinate (0-39)
 * @param y Y coordinate (0-24)
 * @return false if value is 0x55, otherwise true.
 */
bool isCardPresent(unsigned char x, unsigned char y)
{
    unsigned char far *video = VIDEO_RAM_ADDR;
    unsigned char o = 0;

    x <<= 1; // Convert column to video ram offset
    y <<= 3; // Convert row to line
    o = y * VIDEO_LINE_BYTES + x; // compute offset

    return !(video[o] == 0x55); // Return true if background color isn't plot.
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
    unsigned char *suit = NULL; // pointer to suit tile
    unsigned char *val = NULL;  // pointer to value tile
    bool red = false;
    bool adjacentCard = false;

    switch(s[1])
    {
    case 'h':
        suit = &red_card_front[15];
        red=true;
        break;
    case 'd':
        suit = &red_card_front[14];
        red=true;
        break;
    case 'c':
        suit = &black_card_front[15];
        break;
    case 's':
        suit = &black_card_front[14];
        break;
    }

    switch(s[0])
    {
    case '1':
        if (red)
            val = &red_card_front[13];
        else
            val = &black_card_front[13];
        break;
    case '2':
        if (red)
            val = &red_card_front[1];
        else
            val = &black_card_front[1];
        break;
    case '3':
        if (red)
            val = &red_card_front[2];
        else
            val = &black_card_front[2];
        break;
    case '4':
        if (red)
            val = &red_card_front[3];
        else
            val = &black_card_front[3];

        break;
    case '5':
        if (red)
            val = &red_card_front[4];
        else
            val = &black_card_front[4];
        break;
    case '6':
        if (red)
            val = &red_card_front[5];
        else
            val = &black_card_front[5];

        break;
    case '7':
        if (red)
            val = &red_card_front[6];
        else
            val = &black_card_front[6];
        break;
    case '8':
        if (red)
            val = &red_card_front[7];
        else
            val = &black_card_front[7];
        break;
    case '9':
        if (red)
            val = &red_card_front[8];
        else
            val = &black_card_front[8];
        break;
    case 't':
        if (red)
            val = &red_card_front[9];
        else
            val = &black_card_front[9];
        break;
    case 'j':
        if (red)
            val = &red_card_front[10];
        else
            val = &black_card_front[10];
        break;
    case 'q':
        if (red)
            val = &red_card_front[11];
        else
            val = &black_card_front[11];
        break;
    case 'k':
        if (red)
            val = &red_card_front[12];
        else
            val = &black_card_front[12];
        break;
    case '?':
        if (red)
            val = &red_card_front[0];
        else
            val = &black_card_front[0];
        break;
    }

    // Draw top of card
    if (partial == PARTIAL_LEFT)
    {
        plot_tile(&card_edges[0],x,y);
        plot_tile(&card_bits[6],x,y+1);
        plot_tile(&card_bits[8],x,y+2);
        plot_tile(&card_bits[10],x,y+3);
        plot_tile(&card_edges[1],x,y+4);
    }
    else if (partial == PARTIAL_RIGHT)
    {
        // TODO: Double check this.
        plot_tile(&card_edges[4],x,y);
        plot_tile(&card_bits[4],x,y+1);
        plot_tile(&card_bits[5],x,y+2);
        plot_tile(&card_bits[6],x,y+3);
        plot_tile(&card_edges[5],x,y+4);
    }
    else // FULL_CARD, top.
    {
        plot_tile(&card_edges[0],x,y);
        plot_tile(&card_edges[2],x+1,y);
        plot_tile(&card_edges[8],x+2,y);
    }

    //

    // Draw left border only if not overlaying existing card

    // Draw value, white space, and bottom.

}

void drawChip(unsigned char x, unsigned char y)
{
    plot_tile(&chip[0],x,y);
}

void drawBlank(unsigned char x, unsigned char y)
{
    // Not used anywhere.
}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char far *video = VIDEO_RAM_ADDR;
    unsigned char o = 0;

    y <<= 3; // Convert row to line
    x <<= 1; // Convert column to video ram offset

    o = y * VIDEO_LINE_BYTES + x;

    while (w--)
    {
        video[o+w] = 0xAA;         // Color 2
        video[o+w+0x2000] = 0xAA;  // second row
    }
}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char far *video = VIDEO_RAM_ADDR;
    unsigned char o = 0;

    y <<= 3; // Convert row to line
    x <<= 1; // Convert column to video ram offset

    o = y * VIDEO_LINE_BYTES + x;

    while (w--)
    {
        video[o+w] = 0x55;         // Color 1
        video[o+w+0x2000] = 0x55;  // second row
    }
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
}

void drawBorder()
{
}

void drawLogo()
{
}

void initGraphics()
{
    union REGS r;

    // Set graphics mode
    r.h.ah = 0x00;
    r.h.al = 0x04; // 320x200x4
    int86(0x10,&r,&r);

    // Remap palette colors, if possible
    r.h.ah = 0x10;
    r.h.al = 0x00;
    r.h.bl = 1;             // Color 1
    r.h.bh = 10;            // to light green.
    int86(0x10,&r,&r);

    r.h.ah = 0x10;
    r.h.al = 0x00;
    r.h.bl = 2;             // Color 2
    r.h.bh = 4;             // To dark red
    int86(0x10,&r,&r);
}

void waitvsync()
{
    // Wait until we are in vsync
    while (inp(0x3DA) & 0x08);
}

uint8_t cycleNextColor()
{
    return 0; // not used at the moment.
}

void setColorMode()
{
}

#endif /* __WATCOMC__ */
