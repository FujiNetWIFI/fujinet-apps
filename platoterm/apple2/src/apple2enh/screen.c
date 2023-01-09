/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 * DHGR Extensions by Michael Sternberg <mhsternberg at gmail dot com>
 * based on the work by Allen Watson III (Apple Orchard Jan 1984)
 *
 * screen.c - Display output functions
 */

#include <apple2.h>
#include <peekpoke.h>
#include <tgi.h>
#include <stdint.h>
#include <stdlib.h>
#include "../protocol.h"
#include "../screen.h"
#include "dhgr_subs.h"

extern uint8_t font[];
extern uint8_t fontm23[];
extern uint8_t FONT_SIZE_X;
extern uint8_t FONT_SIZE_Y;
extern padBool FastText; /* protocol.c */
extern padPt TTYLoc;
extern uint8_t CharWide;
extern uint8_t CharHigh;

#define outb(addr,val)        (*(addr)) = (val)
#define outw(addr,val)        (*(addr)) = (val)

static uint8_t bp=0;
static uint8_t bd=0;

extern uint16_t mul0375(uint16_t val);

extern void (*io_recv_serial_flow_on)(void);
extern void (*io_recv_serial_flow_off)(void);

// size of scaled PLATO screen
padPt actualSize = {512, 192};

#define FONTPTR(a) (((a << 1) + a) << 1)
#define mul05(a) ((a>>1)+12)

/**
 * screen_load_driver()
 * Load the TGI driver
 */
void screen_load_driver(void)
{
    dg();   /* Initialize DHGR (Double Hi-Res Graphics) */
    pen();  /* Set brush to monochrome */
}

/**
 * screen_init_hook()
 * Called after tgi_init to set any special features, e.g. nmi trampolines.
 */
void screen_init_hook(void)
{
}

/**
 * dhbar()
 * DHGR replacement for tgi_bar()
 */
void dhbar(int x1, int y1, int x2, int y2)
{
    int i;
    if (y2 < y1)
    {
        i = y2;
        y2 = y1;
        y1 = i;
    }
    for (i=y1; i<=y2; i++)
    {
        dot_at(x1, i);
        line_to(x2, i);
    }
}

/**
 * screen_set_pen_mode()
 * Set the pen mode based on CurMode.
 */
void screen_set_pen_mode(void)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    hue(DHGR_COLOR_BLACK);
  else
    hue(DHGR_COLOR_WHITE);
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // Block erase takes forever, manually assert flow control.
  io_recv_serial_flow_off(); 
  
  screen_set_pen_mode();
  dhbar(Coord1->x+24,mul0375(Coord1->y^0x1FF),Coord2->x+24,mul0375(Coord2->y^0x1FF));

  io_recv_serial_flow_on();
  
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  screen_set_pen_mode();
  dot_at(Coord->x+24,mul0375(Coord->y^0x1FF));
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  screen_set_pen_mode();
  dot_at(Coord1->x+24, mul0375(Coord1->y^0x01FF));
  line_to(Coord2->x+24, mul0375(Coord2->y^0x01FF));
}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if ((theChar == 0x0b)) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))	/* backspace */
    {
      TTYLoc.x -= CharWide;

      hue(DHGR_COLOR_BLACK);
      dhbar(TTYLoc.x,mul0375(TTYLoc.y^0x1FF),TTYLoc.x+CharWide,mul0375((TTYLoc.y+CharHigh)^0x1FF));
      hue(DHGR_COLOR_WHITE);
    }
  else if (theChar == 0x0A)			/* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)			/* carriage return */
    TTYLoc.x = 0;
  
  if (TTYLoc.x + CharWide > 511) {	/* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }
  
  if (TTYLoc.y < 0) {
    dhclr(); /* DHGR clear screen */
    TTYLoc.y=495;
  }

}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  // TODO: 60hz wait
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  for (bd=0;bd<75;++bd)
    {
      POKE(0xC030,0);
      for (bp=0;bp<75;++bp)
	{
	}
    }
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  int16_t offset; /* due to negative offsets */
  uint16_t x;      /* Current X and Y coordinates */
  uint16_t y;
  uint16_t* px;   /* Pointers to X and Y coordinates used for actual plotting */
  uint16_t* py;
  uint8_t i; /* current character counter */
  uint8_t a; /* current character byte */
  uint8_t j,k; /* loop counters */
  int8_t b; /* current character row bit signed */
  uint8_t width=FONT_SIZE_X;
  uint8_t height=FONT_SIZE_Y;
  uint16_t deltaX=1;
  uint16_t deltaY=1;
  uint8_t mainColor=DHGR_COLOR_WHITE;
  uint8_t altColor=DHGR_COLOR_BLACK;
  uint8_t *p;
  uint8_t* curfont;
  
  switch(CurMem)
    {
    case M0:
      curfont=font;
      offset=-32;
      break;
    case M1:
      curfont=font;
      offset=64;
      break;
    case M2:
      curfont=fontm23;
      offset=-32;
      break;
    case M3:
      curfont=fontm23;
      offset=32;      
      break;
    }

  if (CurMode==ModeRewrite)
    {
      altColor=DHGR_COLOR_BLACK;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=DHGR_COLOR_WHITE;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=DHGR_COLOR_BLACK;
  else
    mainColor=DHGR_COLOR_WHITE;

  hue(mainColor);

  x=(Coord->x&0x1FF)+24;
  y=mul0375((Coord->y+15^0x1FF)&0x1FF);

  if (ModeBold)
    y=mul0375((Coord->y+30^0x1FF)&0x1FF);
  else
    y=mul0375((Coord->y+15^0x1FF)&0x1FF);

  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  if (CurMode==ModeRewrite)
    {
      goto chardraw_rewrite;
    }

  /* the diet chardraw routine - fast text output. */
  
  hue(mainColor);
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[FONTPTR(a)];
      
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;
	  
  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		  dot_at(x,y);

	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  x-=width;
	  ++p;
  	}

      x+=width;
      y-=height;
    }

  return;

chardraw_rewrite:

  /* the diet chardraw routine - fast text output. */
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[FONTPTR(a)];
      
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;
	  
  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		  hue(mainColor);
	      else
		  hue(altColor);

	      dot_at(x,y);
	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  x-=width;
	  ++p;
  	}

      x+=width;
      y-=height;
    }

  return;

 chardraw_with_fries:
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
      px=&y;
      py=&x;
    }
    else
    {
      px=&x;
      py=&y;
    }
  
  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[FONTPTR(a)];
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;

	  if (Rotate)
	    {
	      px=&y;
	      py=&x;
	    }
	  else
	    {
	      px=&x;
	      py=&y;
	    }

  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  hue(mainColor);
		  if (ModeBold)
		    {
		      dot_at(*px+1,*py);
		      dot_at(*px,*py+1);
		      dot_at(*px+1,*py+1);
		    }
		  dot_at(*px,*py);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      hue(altColor);
		      if (ModeBold)
			{
			  dot_at(*px+1,*py);
			  dot_at(*px,*py+1);
			  dot_at(*px+1,*py+1);
			}
		      dot_at(*px,*py); 
		    }
		}

	      x += deltaX;
  	      b<<=1;
  	    }

	  y+=deltaY;
	  x-=width;
	  ++p;
  	}

      Coord->x+=width;
      x+=width;
      y-=height;
    }

  return;
  
}
