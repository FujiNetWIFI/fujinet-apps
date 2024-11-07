/**
 * @brief screen routines for pmd85
 */

#include <conio.h>
#include <string.h>
// #include <stdlib.h>
// #include <stdio.h>

#include "globals.h"
#include "screen_util.h"

// like screen with border
#define SCR_X0    2
#define SCR_Y0    2
#define SCR_WIDTH  (48 - 2 * SCR_X0)
#define SCR_HEIGHT (32 - 2 * SCR_Y0)

void revers(unsigned char onoff)
{
    if (onoff)
        cputs("\x1Bp");
    else
        cputs("\x1Bq");
}

void screen_init(void)
{
    screen_fill(PATTERN_SOLID | ACE_CYAN);
    screen_set_region(SCR_X0, SCR_Y0, SCR_WIDTH, 1);
    screen_fill_region(PATTERN_SOLID | ACE_GREEN);
    screen_set_region(SCR_X0, SCR_Y0+1, SCR_WIDTH, 9);
    screen_fill_region(PATTERN_SOLID | ACE_WHITE);

    revers(1); 
    textcolor(GREEN);
    gotoxy(SCR_X0 + (SCR_WIDTH-30) / 2, SCR_Y0);
    cputs("#FUJINET 2024 ELECTION RESULTS");
    textcolor(WHITE);
    gotoxy(SCR_X0+1, SCR_Y0+2);
    cputs("TRUMP");
    gotoxy(SCR_X0+1, SCR_Y0+6);
    cputs("HARRIS");

    screen_set_region(SCR_X0, SCR_Y0+11, SCR_WIDTH, 1);
    screen_fill_region(PATTERN_SOLID | ACE_YELLOW);
    screen_set_region(SCR_X0, SCR_Y0+12, SCR_WIDTH, 6);
    screen_fill_region(PATTERN_SOLID | ACE_WHITE);

    textcolor(YELLOW);
    gotoxy(SCR_X0 + (SCR_WIDTH-12) / 2, SCR_Y0 + 11);
    cputs("SENATE SEATS");
    textcolor(WHITE);
    gotoxy(SCR_X0+1, SCR_Y0+13);
    cputs("REPUBLICAN");
    gotoxy(SCR_X0+1, SCR_Y0+16);
    cputs("DEMOCRAT");

    screen_set_region(SCR_X0, SCR_Y0+19, SCR_WIDTH, 1);
    screen_fill_region(PATTERN_SOLID | ACE_MAGENTA);
    screen_set_region(SCR_X0, SCR_Y0+20, SCR_WIDTH, 6);
    screen_fill_region(PATTERN_SOLID | ACE_WHITE);

    textcolor(MAGENTA);
    gotoxy(SCR_X0 + (SCR_WIDTH-10) / 2, SCR_Y0+19);
    cputs("REPS SEATS");
    textcolor(WHITE);
    gotoxy(SCR_X0+1, SCR_Y0+21);
    cputs("REPUBLICAN");
    gotoxy(SCR_X0+1, SCR_Y0+24);
    cputs("DEMOCRAT");

    textcolor(CYAN);
    gotoxy(SCR_X0, SCR_Y0+27);
    cputs("LAST UPDATED:");
}

void draw_bar(unsigned char y, unsigned char w, unsigned char h, unsigned short color)
{
    if (w > SCR_WIDTH) w = SCR_WIDTH;
    if (w)
    {
        screen_set_region(SCR_X0, y, w, h);
        screen_fill_region(PATTERN_SOLID | color);
    } else 
    {
        screen_set_region(SCR_X0, y, 1, h);
        screen_fill_region(0x3E3E | ACE_WHITE);
    }
}

void screen_election(void)
{
    unsigned char rp, dp;
    static char status[64];

    revers(1); 
    textcolor(WHITE);

    rp = (d2 * SCR_WIDTH + 135) / 270;
    draw_bar(SCR_Y0+3, rp, 2, ACE_RED);
    dp = (d1 * SCR_WIDTH + 135) / 270;
    draw_bar(SCR_Y0+7, dp, 2, ACE_BLUE);

    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+2);
    cprintf("%3d", d2);
    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+6);
    cprintf("%3d", d1);

    if (d1>269)
    {
        gotoxy(SCR_X0+20, SCR_Y0+6);
        cputs("WINS!");
    }
    else if (d2>269)
    {
        gotoxy(SCR_X0+20, SCR_Y0+2);
        cputs("WINS!");
    }

    rp = (d4 * SCR_WIDTH + 50) / 100;
    draw_bar(SCR_Y0+14, rp, 1, ACE_RED);
    dp = (d3 * SCR_WIDTH + 50) / 100;
    draw_bar(SCR_Y0+17, dp, 1, ACE_BLUE);

    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+13);
    cprintf("%3d", d4);
    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+16);
    cprintf("%3d", d3);

    rp = (d6 * SCR_WIDTH + 267) / 535;
    draw_bar(SCR_Y0+22, rp, 1, ACE_RED);
    dp = (d5 * SCR_WIDTH + 267) / 535;
    draw_bar(SCR_Y0+25, dp, 1, ACE_BLUE);

    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+21);
    cprintf("%3d", d6);
    gotoxy(SCR_X0+SCR_WIDTH-4, SCR_Y0+24);
    cprintf("%3d", d5);


    textcolor(CYAN);
    gotoxy(SCR_X0+14, SCR_Y0+27);
    cprintf("%s %s    ", d, t);
}
