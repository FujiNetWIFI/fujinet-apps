/**
 * Election
 */

#include <atari.h>
#include <string.h>
#include <peekpoke.h>
#include <stdlib.h>
#include "screen.h"

unsigned char trip=0;

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

extern unsigned char* video_ptr;
extern unsigned char* dlist_ptr;
extern unsigned short screen_memory;
extern unsigned char* font_ptr;

void election(unsigned short r, unsigned char d, char* stamp)
{
  char rs[4]={0,0,0,0};
  char ds[4]={0,0,0,0};

  unsigned char rp,dp,i;
  
  OS.color0=0x0f;
  OS.color1=0x46;
  OS.color2=0x96;
  OS.color4=0x90;
  OS.coldst=1;

  memcpy((void *)DISPLAY_LIST,&config_dlist,sizeof(config_dlist)); // copy display list to $0600
  OS.sdlst=(void *)DISPLAY_LIST;                     // and use it.
  dlist_ptr=(unsigned char *)OS.sdlst;               // Set up the vars for the screen output macros
  screen_memory=PEEKW(560)+4;
  video_ptr=(unsigned char*)(PEEKW(screen_memory));

  // Copy ROM font
  memcpy((unsigned char *)FONT_MEMORY,(unsigned char *)0xE000,1024);

  /* // And patch it. */
  font_ptr=(unsigned char*)FONT_MEMORY;
  memcpy(&font_ptr[464],&fontPatch,sizeof(fontPatch));
  OS.chbas=0x78; // use the charset
  
  screen_clear();
  screen_puts(0,0,"   #FUJINET  2020   ");
  screen_puts(0,1,"  ELECTION RESULTS ");

  screen_puts(0,3,"trump");
  screen_puts(0,6,"\xA2\xA9\xA4\xA5\xAE");

  itoa(r,rs,10);
  itoa(d,ds,10);

  screen_puts(7,3,rs);
  screen_puts(7,6,ds);

  rp=r/16;
  dp=d/16;

  for (i=0;i<rp;i++)
    {
      screen_puts(i,4,"\x7A");
    }

  for (i=0;i<dp;i++)
    {
      screen_puts(i,7,"\xBA");
    }

  screen_puts(0,10,"    LAST UPDATED:   ");
  screen_puts(5,11,stamp);
  for(;;) {}
  
}

void main(void)
{
  election(304,227,"9:59 pm EST");
}
