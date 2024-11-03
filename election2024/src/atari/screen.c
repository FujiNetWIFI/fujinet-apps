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
#define SCREEN_SIZE (20*25)

unsigned char *video_ptr = NULL;

#define SetChar(x,y,a) video_ptr[(x)+(y)*20]=(a);
#define GetChar(x,y) video_ptr[(x)+(y)*20]

unsigned char fontPatch[8]=
  {
   255,255,255,255,255,255,255,255
  };

void config_dlist=
  {
   DL_BLK8,
   DL_BLK8,
   DL_BLK8,
   DL_LMS(DL_CHR20x16x2),
   DISPLAY_MEMORY,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
   DL_CHR20x16x2,
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

    screen_clear();
    screen_puts(0,0,"   #FUJINET  2024   ");
    screen_puts(0,1,"  ELECTION RESULTS ");

    screen_puts(0,3,l2);
    screen_puts(0,6,l1);

    itoa(d2,rs,10);
    itoa(d1,ds,10);

    screen_puts(7,3,rs);
    screen_puts(7,6,ds);

    rp=d2/16;
    dp=d1/16;

    for (i=0;i<rp;i++)
    {
        screen_puts(i,4,"\x7A");
    }

    for (i=0;i<dp;i++)
    {
        screen_puts(i,7,"\xBA");
    }

    screen_puts(0,9,"    LAST UPDATED:   ");
    screen_puts(4,10,d);
    screen_puts(5,11,t);

    if (d1>269)
    {
        screen_puts(12,6,"WINS!");
        for (;;) { OS.color2++; }
    }
    else if (d2>269)
    {
        screen_puts(12,3,"WINS!");
        for (;;) { OS.color1++; }
    }

}
