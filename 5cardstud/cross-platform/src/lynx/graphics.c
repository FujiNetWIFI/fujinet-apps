#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char drawpage;


// Initialize TGI, and screen
void initGraphics()
{
    // Setup TGI
    tgi_install(tgi_static_stddrv);
    tgi_init();
    tgi_setcolor(TGI_COLOR_WHITE);

    drawpage = 0;

    // enable double buffering
    tgi_setviewpage(drawpage);
    tgi_setdrawpage(drawpage);
}

// Reset the screen
void resetScreen()
{
    // clear the screen
    tgi_setbgcolor(TGI_COLOR_GREEN);        // FIXME: allow user to select background color
    tgi_clear();

}

// Draw cards at corners of screen
void drawBorder() {
    
    // draw box around screen
    tgi_setcolor(TGI_COLOR_RED);
    tgi_line(1, 1, 158, 1);         // top line
    tgi_lineto(158, 1, 158, 100);   // right line
    tgi_lineto(158, 100, 1, 100);   // bottom line
    tgi_lineto(1, 100, 1, 1);       // left line

    // draw aces in each corner
}


// Draw 5CS logo/title
// FIXME: on Lynx, maybe we display a full screen sprite with logo?
void drawLogo()
{
    tgi_outtextxy(52, 41, "FUJINET");
    tgi_outtextxy(24, 50, "FIVE CARD STUD");
}


void enableDoubleBuffer();

void disableDoubleBuffer();

void drawBuffer();