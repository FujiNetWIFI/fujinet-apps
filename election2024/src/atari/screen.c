/**
 * @brief screen routines for atari
 */

#include <atari.h>
#include <stdlib.h>
#include <string.h>
#include "screen_election.h"
#include "globals.h"

#define DISPLAY_LIST 0x0600
#define DISPLAY_MEMORY 0x7200
#define FONT_MEMORY 0x7800
#define SCREEN_WIDTH 20
#define SCREEN_SIZE (SCREEN_WIDTH*24)

unsigned char *video_ptr = NULL;

#define SetChar(x,y,a) video_ptr[(x)+(y)*SCREEN_WIDTH]=(a);
#define GetChar(x,y) video_ptr[(x)+(y)*SCREEN_WIDTH]

unsigned char fontPatch[8]=
  {
   255,255,255,255,255,255,255,255
  };

void config_dlist=
  {
   DL_BLK8,
   DL_BLK8,

   DL_LMS(DL_CHR20x8x2),
   DISPLAY_MEMORY,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x16x2,
   DL_CHR20x8x2,
   DL_CHR20x16x2,
   DL_CHR20x8x2,
   DL_BLK2,
   DL_CHR20x8x2,
   DL_BLK2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_BLK2,
   DL_CHR20x8x2,
   DL_BLK2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_CHR20x8x2,
   DL_JVB,
   0x600,
   0,0,0,0
  };

void screen_clear()
{
    memset((unsigned char *)DISPLAY_MEMORY,0x00,SCREEN_SIZE);
}

/**
 * Print ATASCII string to display memory
 */
void screen_puts(unsigned char x,unsigned char y,char *s)
{
    char offset;
  
    do
    {     
        if (*s < 32)
	{
            offset=64;
	}
        else if (*s<96)
	{
            offset=-32;
	}
        else
	{
            offset=0;
	}
      
        SetChar(x++,y,*s+offset);

        ++s;
      
    } while(*s!=0);
}

void draw_bar(unsigned char y, unsigned char w, unsigned char c)
{
    unsigned char i;
    if (w > SCREEN_WIDTH) w = SCREEN_WIDTH;
    for (i = 0; i < w; i++)
        SetChar(i, y, c);
}

void screen_init(void)
{
    unsigned char *fp = (unsigned char *)FONT_MEMORY;
    
    OS.color0=0x0f;
    OS.color1=0x46;
    OS.color2=0x96;
    OS.color4=0x90;
    OS.coldst=1;

    /* Copy in the display list and use */
    memcpy((void *)DISPLAY_LIST,&config_dlist,sizeof(config_dlist));
    OS.sdlst=(void *)DISPLAY_LIST;
    
    /* Make copy of system font and patch for bar graph */
    memcpy((unsigned char *)FONT_MEMORY,(unsigned char *)0xE000,1024);
    memcpy(&fp[464],&fontPatch,sizeof(fontPatch));
    OS.chbas = 0x78; /* 0x7800 */

    /* Set video pointer for the text routine */
    video_ptr = (unsigned char *)DISPLAY_MEMORY;
}

void screen_election(void)
{
    char rs[4]={0,0,0,0};
    char ds[4]={0,0,0,0};

    unsigned char rp,dp,i;

    OS.atract = 0;

    screen_clear();
    screen_puts(0,0,"   #FUJINET  2024   ");
    screen_puts(0,1,"  ELECTION RESULTS ");

    if (strcmp(l1, "ERROR") == 0)
    {
        screen_puts(0,3,"ERROR FETCHING VOTES");
        return;
    }

    screen_puts(0,3,"TRUMP");
    screen_puts(0,5,"HARRIS");

    itoa(d2,rs,10);
    itoa(d1,ds,10);

    screen_puts(SCREEN_WIDTH-strlen(rs),3,rs);
    screen_puts(SCREEN_WIDTH-strlen(ds),5,ds);

    rp = (d2 * SCREEN_WIDTH + 135) / 270;
    dp = (d1 * SCREEN_WIDTH + 135) / 270;
    draw_bar(4, rp, '\x7A');
    draw_bar(6, dp, '\xBA');

    screen_puts(0,8,"    SENATE SEATS    ");

    screen_puts(0,9,"REPUBLICAN");
    itoa(d4,rs,10);
    screen_puts(SCREEN_WIDTH-strlen(rs),9,rs);
    
    screen_puts(0,11,"DEMOCRAT");
    itoa(d3,ds,10);
    screen_puts(SCREEN_WIDTH-strlen(ds),11,ds);

    rp = (d4 * SCREEN_WIDTH + 50) / 100;
    dp = (d3 * SCREEN_WIDTH + 50) / 100;
    draw_bar(12, dp, '\xBA');
    draw_bar(10, rp, '\x7A');

    screen_puts(0,14,"     REPS SEATS     ");

    screen_puts(0,15,"REPUBLICAN");
    itoa(d6,rs,10);
    screen_puts(SCREEN_WIDTH-strlen(rs),15,rs);

    screen_puts(0,17,"DEMOCRAT");
    itoa(d5,ds,10);
    screen_puts(SCREEN_WIDTH-strlen(ds),17,ds);

    rp = (d6 * SCREEN_WIDTH + 267) / 535;
    dp = (d5 * SCREEN_WIDTH + 267) / 535;
    draw_bar(16, rp, '\x7A');
    draw_bar(18, dp, '\xBA');
        
    screen_puts(4,20,d);
    screen_puts(0,21,t);
    screen_puts(11,21,newsite);

    if (d1>269)
    {
        screen_puts(9,5,"WINS!");
        /* for (;;) { OS.color2++; } */
    }
    else if (d2>269)
    {
        screen_puts(9,3,"WINS!");
        /* for (;;) { OS.color1++; } */
    }

}
