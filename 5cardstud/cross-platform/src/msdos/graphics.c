#ifdef __WATCOMC__

#include <stdint.h>
#include <string.h>
#include "charset.h"

#define VIDEO_RAM_ADDR ((unsigned char far *)0xB8000000UL)
#define VIDEO_LINE_BYTES 80
#define VIDEO_ODD_OFFSET 0x2000

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
void drawText(unsigned char x, unsigned char y, char *s)
{
    signed char c=0;

    while (c = *s++)
    {
        c -= 32;
        if (c<0 || c>96)
            continue;

        if (x>39)
            continue;
        else
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
}

void drawStatusTextAt(unsigned char x, char* s)
{
}

void drawStatusText(char* s)
{
}

void drawStatusTimer()
{
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
}

void drawChip(unsigned char x, unsigned char y)
{
}

void drawBlank(unsigned char x, unsigned char y)
{
}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
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
}

void waitvsync()
{
}

uint8_t cycleNextColor()
{
}

void setColorMode()
{
}

#endif /* __WATCOMC__ */
