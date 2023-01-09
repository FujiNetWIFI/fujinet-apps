/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen_base.c - Display output functions (base)
 */

#include <string.h>
#include <stdbool.h>
#include <tgi.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <stdio.h>
#include "screen.h"
#include "protocol.h"
#include "io.h"
#ifdef __APPLE2ENH__
#include "./apple2enh/dhgr_subs.h"
#endif

uint8_t CharWide=8;
uint8_t CharHigh=16;
padPt TTYLoc;
uint8_t pal[2];
unsigned char current_foreground=COLOR_WHITE;
unsigned char current_background=COLOR_BLACK;

extern uint8_t xoff_enabled; /* io.c */
extern padBool FastText; /* protocol.c */
extern uint8_t recv_buffer[384];
extern uint8_t font[];
extern uint8_t fontm23[];
extern uint8_t FONT_SIZE_X;
extern uint8_t FONT_SIZE_Y;

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  screen_load_driver();
#ifdef __APPLE2ENH__
  screen_init_hook();
#else  
  tgi_init();
  screen_init_hook();
  tgi_clear();
#endif
}

#ifndef __ATARI__
/**
 * screen_splash - Show splash screen
 */
void screen_splash(void)
{
  FILE *fp;
  short len=0;

  fp=fopen("splash.bin","r");

  while (len=fread(recv_buffer,sizeof(unsigned char),384,fp))
    ShowPLATO((padByte *)recv_buffer,len);

  memset(recv_buffer,0,sizeof(recv_buffer));
  
  fclose(fp);
}
#endif /* ifndef __ATARI__ */

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
#ifdef __APPLE2ENH__
  dhclr();
#else
  tgi_clear();
#endif
  screen_update_colors();
}

/**
 * match_color - quantize to nearest system palette color.
 * will eventually do a full hsv match to nearest possible color,
 * but for now, it's a straight match of system colors only.
 */
unsigned char screen_match_color(padRGB* theColor)
{
#ifdef __C64__
  if (theColor->red==0 && theColor->green==0 && theColor->blue==0)
    {
      return COLOR_BLACK;
    }
  else if (theColor->red==0 && theColor->green==0 && theColor->blue==255)
    {
      return COLOR_BLUE;
    }
  else if (theColor->red==0 && theColor->green==255 && theColor->blue==0)
    {
      return COLOR_GREEN;
    }
  else if (theColor->red==255 && theColor->green==0 && theColor->blue==0)
    {
      return COLOR_RED;
    }
  else if (theColor->red==0 && theColor->green==255 && theColor->blue==255)
    {
      return COLOR_CYAN;
    }
  else if (theColor->red==255 && theColor->green==0 && theColor->blue==255)
    {
      return COLOR_LIGHTRED;
    }
  else if (theColor->red==255 && theColor->green==255 && theColor->blue==0)
    {
      return COLOR_YELLOW;
    }
#endif
  // For any other color, return white.
  return COLOR_WHITE;
}

/**
 * screen_foreground - set foreground color
 */
void screen_foreground(padRGB* theColor)
{
  current_foreground=screen_match_color(theColor);
}

/**
 * screen_background - set background_color
 */
void screen_background(padRGB* theColor)
{
  current_background=screen_match_color(theColor);
}

/**
 * screen_paint - Paint screen scanline_fill
 */
void _screen_paint(unsigned short x, unsigned short y)
{
/* #ifndef __APPLE2__ */
/*   // Currently we run out of memory on apple2, so this is temporarily disabled. */
/*   static unsigned short xStack[64]; */
/*   static unsigned char yStack[64]; */
/*   unsigned char stackentry = 1; */
/*   unsigned char spanAbove, spanBelow; */
  
/*   unsigned char oldColor = tgi_getpixel(x,y); */
  
/*   if (oldColor == 1) */
/*     return; */
  
/*   do */
/*     { */
/*       while (x > 0 && tgi_getpixel(x-1,y) == oldColor) */
/* 	--x; */
      
/*       spanAbove = spanBelow = false; */
/*       while(tgi_getpixel(x,y) == oldColor) */
/* 	{ */
/* 	  tgi_setpixel(x,y); */
	  
/* 	  if (y < (191)) */
/* 	    { */
/* 	      unsigned char belowColor = tgi_getpixel(x, y+1); */
/* 	      if (!spanBelow  && belowColor == oldColor) */
/* 		{ */
/* 		  xStack[stackentry]  = x; */
/* 		  yStack[stackentry]  = y+1;					 */
/* 		  ++stackentry; */
/* 		  spanBelow = true; */
/* 		} */
/* 	      else if (spanBelow && belowColor != oldColor) */
/* 		spanBelow = false; */
/* 	    } */
	  
/* 	  if (y > 0) */
/* 	    { */
/* 	      unsigned char aboveColor = tgi_getpixel(x, y-1); */
/* 	      if (!spanAbove  && aboveColor == oldColor) */
/* 		{ */
/* 		  xStack[stackentry]  = x; */
/* 		  yStack[stackentry]  = y-1; */
/* 		  ++stackentry; */
/* 		  spanAbove = true; */
/* 		} */
/* 	      else if (spanAbove && aboveColor != oldColor) */
/* 		spanAbove = false; */
/* 	    } */
	  
/* 	  ++x; */
/* 	} */
/*       --stackentry; */
/*       x = xStack[stackentry]; */
/*       y = yStack[stackentry]; */
/*     } */
/*   while (stackentry); */
/* #endif */
}

/**
 * screen_paint - Paint the screen
 * Calling convention is different due to this needing to be recursive.
 */
void screen_paint(padPt* Coord)
{
/*   if (config.fill==true) */
/*     { */
/* #ifdef __ATARI__ */
/*       _screen_paint(mul0625(Coord->x),mul0375(Coord->y^0x1FF)); */
/* #else */
/*       _screen_paint(scalex[Coord->x],scaley[Coord->y]); */
/* #endif */
/*     } */
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
#ifndef __APPLE2ENH__
  tgi_done();
  tgi_uninstall();
#endif
}
