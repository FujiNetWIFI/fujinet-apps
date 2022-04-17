#include <conio.h>
#include <msx.h>
#include <graphics.h>
#include <smartkeys.h>
#include "screen.h"
#include "font.h"
#include "protocol.h"
#include "math.h"
#include "io.h"
#include "sound.h"

#include "font.h"
#include "protocol.h"
#include "math.h"
#include "io.h"
#include "sound.h"

unsigned char CharWide=8;
unsigned char CharHigh=16;
padPt TTYLoc;
padPt statusLoc={0,0};
unsigned char current_foreground=INK_WHITE;
unsigned char current_background=INK_BLACK;
extern padBool FastText; /* protocol.c */
extern unsigned char font[];
extern unsigned char fontm23[];
extern unsigned char FONT_SIZE_X;
extern unsigned char FONT_SIZE_Y;

// Miscellaneous math functions needed for coordinate translation.
short max(short a, short b) { return ( a > b ) ? a : b; }
short min(short a, short b) { return ( a < b ) ? a : b; }
short scaley(short y) { return (((y^0x01FF) << 1) * 3) >> 4; }
short scalex(short x) { return (x >> 1); }

#define FONTPTR(a) (a*6)

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  msx_color(current_foreground,current_background,current_background);
  clrscr();
  smartkeys_sound_init();
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  msleep(17); 
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  smartkeys_sound_play(SOUND_POSITIVE_CHIME);
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  clrscr();
}

/**
 * screen_set_pen_mode(void) - Set the current pen mode
 */
void screen_set_pen_mode(void)
{
  msx_color(current_foreground,current_background,current_background);
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  int mode;
  int x1=min(scalex(Coord1->x),scalex(Coord2->x));
  int x2=max(scalex(Coord1->x),scalex(Coord2->x));
  int y1=min(scaley(Coord1->y),scaley(Coord2->y));
  int y2=max(scaley(Coord1->y),scaley(Coord2->y));
  int y;
  
  screen_set_pen_mode();

  if (CurMode==ModeErase || CurMode==ModeInverse)
    {
      for (y=y1;y<y2;y++)
	undraw(x1,y,x2,y);
    }
  else
    {
      for(y=y1;y<y2;y++)
	draw(x1,y,x2,y);
    }
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  screen_set_pen_mode();
  if (CurMode==ModeErase)
    unplot(scalex(Coord->x),scaley(Coord->y));
  else
    plot(scalex(Coord->x),scaley(Coord->y));
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  unsigned short x1=scalex(Coord1->x);
  unsigned short x2=scalex(Coord2->x);
  unsigned short y1=scaley(Coord1->y);
  unsigned short y2=scaley(Coord2->y);
  
  screen_set_pen_mode();
  if (CurMode==ModeErase)
    undraw(x1,y1,x2,y2);
  else
    draw(x1,y1,x2,y2);

}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  short offset; /* due to negative offsets */
  int x;      /* Current X and Y coordinates */
  int y;
  int* px;   /* Pointers to X and Y coordinates used for actual plotting */
  int* py;
  unsigned char i; /* current character counter */
  unsigned char a; /* current character byte */
  unsigned char j,k; /* loop counters */
  char b; /* current character row bit signed */
  unsigned char width=FONT_SIZE_X;
  unsigned char height=FONT_SIZE_Y;
  unsigned short deltaX=1;
  unsigned short deltaY=1;
  unsigned char mainColor=current_foreground;
  unsigned char altColor=current_background;
  unsigned char *p;
  unsigned char* curfont;
  
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
      altColor=current_background;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=current_foreground;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=current_background;
  else
    mainColor=current_foreground;

  msx_color(mainColor,current_background,current_background);
  
  x=scalex((Coord->x&0x1FF));

  if (ModeBold)
    y=scaley((Coord->y+30)&0x1FF);
  else
    y=scaley((Coord->y+15)&0x1FF);

  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

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
		{
		  msx_color(mainColor,current_background,current_background);
		  if (CurMode==ModeErase)
		    unplot(x,y);
		  else
		    plot(x,y);
		}

	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  if (y>191)
	    y-=192;
	  x-=width;
	  ++p;
  	}

      x+=width;
      y-=height;
      if (y<0)
	y=192+y;
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

	  // special 9918 specific hack for inverse video
	  if (CurMode==ModeInverse)
	    {
	      for (k=0;k<FONT_SIZE_X;++k)
	  	{
	  	  if (ModeBold)
	  	    {
	  	      plot(*px+1,*py);
	  	      plot(*px,*py+1);
	  	      plot(*px+1,*py+1);
	  	    }
	  	  plot(*px,*py);
	  	}
	    }
	  
  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  if (ModeBold)
		    {
		      plot(*px+1,*py);
		      plot(*px,*py+1);
		      plot(*px+1,*py+1);
		    }
		  plot(*px,*py);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      if (ModeBold)
			{
			  unplot(*px+1,*py);
			  unplot(*px,*py+1);
			  unplot(*px+1,*py+1);
			}
		      unplot(*px,*py); 
		    }
		}

	      x += deltaX;
  	      b<<=1;
  	    }

	  y+=deltaY;
	  if (y>191)
	    y-=192;
	  x-=width;
	  ++p;
  	}

      Coord->x+=width;
      x+=width;
      y-=height;

      if (y<0)
	y=192+y;
    }

  return;  
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
      padPt ec1,ec2;
      TTYLoc.x -= CharWide;
      ec1.x=TTYLoc.x;
      ec1.y=TTYLoc.y;
      ec2.x=TTYLoc.x+CharWide;
      ec2.y=TTYLoc.y+CharHigh;
      screen_block_draw(&ec1,&ec2);
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
    screen_clear();
    TTYLoc.y=495;
  }

}

/**
 * screen_color - return closest match to requested color.
 */
unsigned char screen_color(padRGB* theColor)
{
  unsigned char red=theColor->red;
  unsigned char green=theColor->green;
  unsigned char blue=theColor->blue;

  if (red==0 && green==0 && blue==0)
    {
      return INK_BLACK;
    }
  else if (red==0 && green==0 && blue==255)
    {
      return INK_LIGHT_BLUE;
    }
  else if (red==0 && green==255 && blue==0)
    {
      return INK_LIGHT_GREEN;
    }
  else if (red==255 && green==0 && blue==0)
    {
      return INK_LIGHT_RED;
    }
  else if (red==0 && green==255 && blue==255)
    {
      return INK_CYAN;
    }
  else if (red==255 && green==0 && blue==255)
    {
      return INK_MAGENTA;
    }
  else if (red==255 && green==255 && blue==0)
    {
      return INK_LIGHT_YELLOW;
    }
  else
    {
      return INK_WHITE;
    }
}


/**
 * screen_foreground - Called to set foreground color.
 */
void screen_foreground(padRGB* theColor)
{
  current_foreground = screen_color(theColor);
}

/**
 * screen_background - Called to set foreground color.
 */
void screen_background(padRGB* theColor)
{
  /* current_background=screen_color(theColor); */
}

/**
 * screen_paint - Called to paint at location.
 */
void screen_paint(padPt* Coord)
{
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
}
