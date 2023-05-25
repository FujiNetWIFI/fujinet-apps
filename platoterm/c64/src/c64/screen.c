/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * screen.c - Display output functions
 */

#include <c64.h>
#include <cbm.h>
#include <peekpoke.h>
#include <tgi.h>
#include <stdint.h>
#include <stdlib.h>
#include "../config.h"
#include "../protocol.h"
#include "../screen.h"

extern uint8_t pal[2];
extern ConfigInfo config; 

extern unsigned char current_foreground;
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

extern void install_nmi_trampoline(void); /* nmi_trampoline.s */
extern void color_transform(void); /* color_transform.s */
unsigned short colorpt;
unsigned short x_coord;
unsigned char y_coord;

extern uint16_t mul0625(uint16_t val);
extern uint16_t mul0375(uint16_t val);

extern void (*io_recv_serial_flow_on)(void);
extern void (*io_recv_serial_flow_off)(void);

// size of scaled PLATO screen
padPt actualSize = {320, 192};

#define FONTPTR(a) (((a << 1) + a) << 1)

/**
 * screen_load_driver()
 * Load the TGI driver
 */
void screen_load_driver(void)
{
  tgi_install(tgi_static_stddrv);
}

/**
 * screen_init_hook()
 * Called after tgi_init to set any special features, e.g. nmi trampolines.
 */
void screen_init_hook(void)
{
  install_nmi_trampoline();
}

/**
 * screen_set_pen_mode()
 * Set the pen mode based on CurMode.
 */
void screen_set_pen_mode(void)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    tgi_setcolor(TGI_COLOR_BLACK);
  else
    tgi_setcolor(TGI_COLOR_WHITE);
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // Block erase takes forever, manually assert flow control.
  io_recv_serial_flow_off(); 

  screen_set_pen_mode();
  tgi_bar(mul0625(Coord1->x),mul0375(Coord1->y^0x1FF),mul0625(Coord2->x),mul0375(Coord2->y^0x1FF));
  
  io_recv_serial_flow_on();
  
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  screen_set_pen_mode();
  tgi_setpixel(mul0625(Coord->x),mul0375(Coord->y^0x1FF));
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  screen_set_pen_mode();
  tgi_line(mul0625(Coord1->x),mul0375(Coord1->y^0x1FF),mul0625(Coord2->x),mul0375(Coord2->y^0x1FF));
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

      tgi_setcolor(TGI_COLOR_BLACK);
      tgi_bar(mul0625(TTYLoc.x),mul0375(TTYLoc.y^0x1FF),mul0625(TTYLoc.x+CharWide),mul0375((TTYLoc.y+CharHigh)^0x1FF));
      tgi_setcolor(TGI_COLOR_WHITE);
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
    tgi_clear();
    TTYLoc.y=495;
  }

}

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
  ++config.color_foreground;
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
  ++config.color_background;
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 */
void screen_cycle_border(void)
{
  ++config.color_border;
}

/**
 * Set the terminal colors
 */
void screen_update_colors(void)
{
  pal[0]=config.color_background;
  pal[1]=config.color_foreground;
  tgi_setpalette(pal);
  POKE(0xD020,config.color_border);
}

/**
 * Wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
  waitvsync();
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  // My feeble attempt at a beep.
  outw(&SID.v1.freq,0x22cd);
  outw(&SID.v1.pw,0x0800);
  outb(&SID.v1.ad,0x33);
  outb(&SID.v1.sr,0xF0);
  outb(&SID.amp,0x5F);
  outw(&SID.flt_freq,0xF0F0);
  outb(&SID.flt_ctrl,0xF2);
  outb(&SID.v1.ctrl,0x11);
  waitvsync();
  waitvsync();
  waitvsync();
  waitvsync();
  outb(&SID.v1.ctrl,0);
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
  uint8_t mainColor=1;
  uint8_t altColor=0;
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
      altColor=0;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=1;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=0;
  else
    mainColor=1;

  tgi_setcolor(mainColor);

  // Set initial coordinate positions, and adjust for bold if needed.
  x=mul0625((Coord->x&0x1FF));
  
  if (ModeBold)
    y=mul0375((Coord->y+30^0x1FF)&0x1FF);
  else
    y=mul0375((Coord->y+15^0x1FF)&0x1FF);
  
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
		  tgi_setcolor(mainColor);
		  tgi_setpixel(x,y);
#ifdef __C64_COLOR__
		  x_coord=x-4;
		  y_coord=y;
		  color_transform(); // take x_coord/y_coord, return offset into color ram in colorpt.
		  asm("sei");          // stop interrupts.
		  asm("lda $01");      // value of kernal banking address
		  asm("pha");          // push onto stack
		  asm("and #$FC");     // AND off last two bits (to page out kernal and basic roms)
		  asm("sta $01");      // and pop back into kernal banking address
		  POKE(0xD000+colorpt,(current_foreground << 4) | PEEK(0xD000+colorpt));   // Plop on new color ram value
		  asm("pla");          // Pop old kernal banking value off stack back into A
		  asm("sta $01");      // store
		  asm("cli");          // and turn back on interrupts, like nothing ever happened.
#endif
		}

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
		  tgi_setcolor(mainColor);
		  if (ModeBold)
		    {
		      tgi_setpixel(*px+1,*py);
		      tgi_setpixel(*px,*py+1);
		      tgi_setpixel(*px+1,*py+1);
#ifdef __C64_COLOR__
		      x_coord=x-4;
		      y_coord=y;
		      color_transform(); // take x_coord/y_coord, return offset into color ram in colorpt.
		      asm("sei");          // stop interrupts.
		      asm("lda $01");      // value of kernal banking address
		      asm("pha");          // push onto stack
		      asm("and #$FC");     // AND off last two bits (to page out kernal and basic roms)
		      asm("sta $01");      // and pop back into kernal banking address
		      POKE(0xD000+colorpt,(current_foreground << 4) | PEEK(0xD000+colorpt));   // Plop on new color ram value
		      asm("pla");          // Pop old kernal banking value off stack back into A
		      asm("sta $01");      // store
		      asm("cli");          // and turn back on interrupts, like nothing ever happened.
#endif
		    }
		  tgi_setpixel(*px,*py);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      tgi_setcolor(altColor);
		      if (ModeBold)
			{
			  tgi_setpixel(*px+1,*py);
			  tgi_setpixel(*px,*py+1);
			  tgi_setpixel(*px+1,*py+1);
			}
		      tgi_setpixel(*px,*py); 
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
