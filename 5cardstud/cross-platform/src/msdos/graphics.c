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
#define GREEN_MASK 0x55

bool always_render_full_cards = 0;

uint8_t colorMode=0;
uint8_t mask=0;
uint8_t textMask=GREEN_MASK;
uint8_t far *video = VIDEO_RAM_ADDR;
uint8_t double_buffer[0x4000];
uint8_t draw_corners=1;
unsigned char oldmode=0;

/**
 * @brief plot a 8x8 2bpp tile to screen at column x, row y
 * @param tile ptr to 2bpp tile data * 8
 * @param x Column 0-39
 * @param y Row 0-24
 */
void plot_tile(const unsigned char *tile, unsigned char x, unsigned char y)
{
    unsigned char i=0;

    if (y<25)
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

    _fmemset(&video[0x0000], b, 8000-640);
    _fmemset(&video[0x2000], b, 8000-640);
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

        plot_tile(&ascii[c+mask],x++,y);
    }

}


void enableDoubleBuffer()
{
    video=&double_buffer[0];
    _fmemcpy(video,VIDEO_RAM_ADDR,0x4000);
}

void disableDoubleBuffer()
{
    drawBuffer();
    video = VIDEO_RAM_ADDR;
}

void drawBuffer()
{
    if (video != VIDEO_RAM_ADDR)
        _fmemcpy(VIDEO_RAM_ADDR,video,0x4000);
}

// Call to clear the screen to an empty table
void resetScreen()
{
    clear(1); // Draw table color across whole screen
    plot_tile(&border[0],0,0);
    plot_tile(&border[1],39,0);
    plot_tile(&border[2],0,22);
    plot_tile(&border[3],39,22);
}

void clearStatusBar()
{

    _fmemset(&video[0x1CC0],0x00,640);
    _fmemset(&video[0x1CC0+0x2000],0x00,640);
}

void drawStatusTextAt(unsigned char x, char* s)
{
    mask=96;
    drawText(x,188,s);
    mask=0;
}

void drawStatusText(char* s)
{
    char* comma;
    clearStatusBar();

    // Wrap double line status text at comma
    if (strlen(s)>40)
    {
        comma = s;
        while (*comma++!=',');
        comma[0]=0;
        comma++;

        mask=96;
        drawText(0, 184, s);
        drawText(0, 192, comma);
        mask=0;
    }
    else
    {
        // otherwise draw it centered vertically
        drawStatusTextAt(0, s);
    }
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
uint8_t getPix(unsigned char x, unsigned char y)
{

    uint16_t o = 0;

    x <<= 1; // Convert column to video ram offset
   // y <<= 3; // Convert row to line
    o = y * 4 * VIDEO_LINE_BYTES + x; // compute offset

    return video[o];
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
    unsigned char *suit = NULL; // pointer to suit tile
    unsigned char *val = NULL;  // pointer to value tile
    bool red = false;
    bool adjacentCard = false;
    bool showCardBack = s[0]=='?';
    uint8_t prevPix;

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
     case 'a':
        if (red)
            val = &red_card_front[13];
        else
            val = &black_card_front[13];
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
        // // TODO: Double check this.
        plot_tile(&card_edges[4],x+1,y);
        plot_tile(&card_bits[1],x+1,y+1);
        plot_tile(&card_bits[2],x+1,y+2);
        plot_tile(&card_bits[3],x+1,y+3);
        plot_tile(&card_edges[5],x+1,y+4);

        plot_tile(&card_edges[8],x+2,y);
        plot_tile(&card_edges[6],x+2,y+1);
        plot_tile(&card_edges[6],x+2,y+2);
        plot_tile(&card_edges[6],x+2,y+3);
        plot_tile(&card_edges[7],x+2,y+4);
    }
    else // FULL_CARD, top.
    {
        plot_tile(&card_edges[0],x,y);
        plot_tile(&card_edges[2],x+1,y);
        prevPix = getPix(x+2,y+2);

        // Draw right border only if not overlaying existing card
        if (prevPix == 0x55)
        {
            // Background color 0x55) , so no card is there. Draw right border
            plot_tile(&card_edges[6],x+2,y+1);
            plot_tile(&card_edges[6],x+2,y+2);
            plot_tile(&card_edges[6],x+2,y+3);

            // Draw top and bottom right corners if not supressed
            // This is only suppressed for the 4 aces in the borders
            // since those areas are used for corners
            if (draw_corners) {
                plot_tile(&card_edges[8],x+2,y);
                plot_tile(&card_edges[7],x+2,y+4);
            }
        }
        else if (prevPix== 0xEB)
        {
            // Back of card ia there, update to draw with left border
            // This happens for the player cards on the right of the screen
            plot_tile(&card_edges[4],x+2,y);
            plot_tile(&card_bits[1],x+2,y+1);
            plot_tile(&card_bits[2],x+2,y+2);
            plot_tile(&card_bits[3],x+2,y+3);
            plot_tile(&card_edges[5],x+2,y+4);
        }


        plot_tile(&card_edges[1],x,y+4);
        plot_tile(&card_edges[3],x+1,y+4);

        // Draw value, white space, and bottom.
        if (!showCardBack)
        {
            plot_tile(&card_bits[0],x,y+1);
            plot_tile(val,x+1,y+1);

            // Middle - draw strip to designate this player's hidden card
            if (isHidden)
            {
                // Alternate - keeping just in case
                // plot_tile(&card_bits[14],x,y+1);
                // plot_tile(&card_bits[15],x,y+2);
                // plot_tile(&card_bits[16],x,y+3);
                // //plot_tile(&card_bits[12],x,y+2);
                // plot_tile(&card_bits[13],x+1,y+2);

                plot_tile(&card_bits[0],x,y+2);
                plot_tile(&red_card_front[0],x+1,y+2);
                plot_tile(&card_bits[0],x,y+3);

                plot_tile(&card_bits[12],x,y+2);
                plot_tile(&card_bits[13],x+1,y+2);

            }
            else
            {
                plot_tile(&card_bits[0],x,y+2);
                plot_tile(&red_card_front[0],x+1,y+2);
                plot_tile(&card_bits[0],x,y+3);

            }
            plot_tile(suit,x+1,y+3);

        }
        else
        {
            // Show back of card (it is flipped - other player's hidden card)
            plot_tile(&card_bits[6],x,y+1);
            plot_tile(&card_bits[7],x+1,y+1);

            plot_tile(&card_bits[8],x,y+2);
            plot_tile(&card_bits[9],x+1,y+2);

            plot_tile(&card_bits[10],x,y+3);
            plot_tile(&card_bits[11],x+1,y+3);
        }
    }
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

    uint16_t o = 0;
    x <<=1;
    w <<= 1;
    o = y * 4 * VIDEO_LINE_BYTES + x;

    // Adjust for bottom move underlines
    if (y==24)
        o+=2*VIDEO_LINE_BYTES;

    _fmemset(&video[o], 0xAA, w);
    _fmemset(&video[o+0x2000], 0xAA, w);

}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{

    uint8_t val = y<22 ? 0x55 : 0;

    uint16_t o = 0;
    x <<=1;
    w <<= 1;
    o = y * 4 * VIDEO_LINE_BYTES + x;

    // Adjust for bottom move underlines
    if (y==24)
        o+=2*VIDEO_LINE_BYTES;

    _fmemset(&video[o],val, w);
    _fmemset(&video[o+0x2000], val, w);
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    uint8_t i;

    plot_tile(&pot_border[0],x,y);
    plot_tile(&pot_border[2],x+w+1,y);
    for (i=1;i<=w;i++) {
        plot_tile(&pot_border[1],x+i,y);
        plot_tile(&pot_border[1],x+i,y+h+1);
    }

    for (i=1;i<=h;i++) {
        plot_tile(&pot_border[5],x,y+i);
        plot_tile(&pot_border[5],x+w+1,y+i);
    }

    plot_tile(&pot_border[3],x,y+h+1);
    plot_tile(&pot_border[4],x+w+1,y+h+1);



}

void drawBorder()
{
  draw_corners=0;
  drawCard(1,0,FULL_CARD, "as", 0);
  drawCard(37,0,FULL_CARD, "ah", 0);
  drawCard(1,18,FULL_CARD, "ad", 0);
  drawCard(37,18,FULL_CARD, "ac", 0);
  draw_corners=1;
}

void drawLogo()
{
}

void initGraphics()
{
    union REGS r;
    uint8_t *c;
    uint16_t i;

    // Get old mode
    r.h.ah = 0x0f;
    int86(0x10,&r,&r);

    oldmode=r.h.al;

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

    // Create a second set of white on black text for status bar
    // It could also be achieved via a mask in exchange for more memory
    c=&ascii[0][0];
    for(i=0;i<sizeof(ascii)/2;i++) {
        // Set  ascii characters to white on black
        *(c+1536)= (*c^0x55)*3;
        c++;
    }

}

void waitvsync()
{
    // Wait until we are in vsync
    while (! (inp(0x3DA) & 0x08));
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
