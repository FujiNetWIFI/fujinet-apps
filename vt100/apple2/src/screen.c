/**
 * 80 column routines
 * 
 * Screen routines
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2.h>
#include <stdbool.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include "screen.h"

/* Soft Switches */
#define SWITCH_40STORE (*(unsigned char *)0xC000)
#define SWITCH_80STORE (*(unsigned char *)0xC001)
#define SWITCH_RDMAINRAM (*(unsigned char *)0xC002)
#define SWITCH_RDCARDRAM (*(unsigned char *)0xC003)
#define SWITCH_WRMAINRAM (*(unsigned char *)0xC004)
#define SWITCH_WRCARDRAM (*(unsigned char *)0xC005)
#define SWITCH_40COL   (*(unsigned char *)0xC00C)
#define SWITCH_80COL   (*(unsigned char *)0xC00D)
#define SWITCH_NORCHAR (*(unsigned char *)0xC00E)
#define SWITCH_ALTCHAR (*(unsigned char *)0xC00F)
#define SWITCH_PAGE1   (*(unsigned char *)0xC054)
#define SWITCH_PAGE2   (*(unsigned char *)0xC055)
#define BLANK_CHAR     0xA0

/* Tabs */
static const unsigned char _tabs[9] = {8,16,24,32,40,48,56,64,72};

/* Screen row table. Row table */
static const unsigned short screen_addr_row[24] =
  {
    0x400,
    0x480,
    0x500,
    0x580,
    0x600,
    0x680,
    0x700,
    0x780,
    0x428,
    0x4A8,
    0x528,
    0x5A8,
    0x628,
    0x6A8,
    0x728,
    0x7A8,
    0x450,
    0x4D0,
    0x550,
    0x5D0,
    0x650,
    0x6D0,
    0x750,
    0x7D0
  };

/* Row/column pointers */
unsigned char _row = 0;
unsigned char _col = 0;
bool _bottom = false;
bool _inverse = false;

/**
 * @brief swap in screen
 */
void screen_use(void)
{
  SWITCH_80STORE=true;
}

/**
 * @brief swap out screen
 */
void screen_unuse(void)
{
  SWITCH_PAGE1=true;
  SWITCH_40STORE=true;
}

/**
 * @brief sound bell
 */
void screen_bell(void)
{
  screen_unuse();
  __asm__("LDA $C082");
  __asm__("JSR $FBDD");
  screen_use();
}

/**
 * @brief do backspace
 */
void screen_bs(void)
{
  
  _col--;
  if (_col==255)
    {
      _col=79;
      _row--;
      
      if (_row==255)
	_row=0;
    }
  
  screen_putcxy(_col,_row,0x20);
  
}

/**
 * @brief do linefeeds
 */
void screen_lf(void)
{
  
  if (_row==23) /* At bottom, do scroll. */
    {
      screen_scroll_up();
      _row=23;
      return;
    }
  else
    screen_cursor_down(1);

  
}

/**
 * @brief do carriage return
 */
void screen_cr(void)
{  
  _col=0; 
}

/**
 * @brief go to next TAB
 */
void screen_tab(void)
{
  unsigned char i=0;
  
  while(_col > _tabs[i++]);

  _col = _tabs[--i];
}

/**
 * @brief clear to end of line
 */
void screen_clear_to_end_of_line(void)
{
  screen_clear_line(_col,_row,80-_col);
}

/**
 * @brief clear from beginning to cursor
 */
void screen_clear_beg_to_line(void)
{
  screen_clear_line(0,_row,_col);
}

/**
 * @brief clear from current pos to end of screen
 */
void screen_clear_cursor_to_end(void)
{
  unsigned char r;

  for (r=_row+1;r<24;r++)
    screen_clear_line(0,r,80);

  screen_clear_line(_col,_row,80-_col);
}

/**
 * @brief clear from beginning to cursor
 */
void screen_clear_beg_to_cursor(void)
{
  unsigned char r;

  if ((_row-1) > 0)
    {
      for (r=0;r<_row;r++)
	screen_clear_line(0,r,80);
    }

  screen_clear_beg_to_line();
}

/**
 * @brief Clear current line
 */
void screen_clear_current_line(void)
{
  screen_clear_line(0,_row,80);
}

/**
 * @brief  Get screen address and set page softswitch
 * @param  x Column
 * @param  y Row
 * @return Destination address.
 */
unsigned short screen_addr(unsigned char x, unsigned char y)
{
  SWITCH_80STORE=true;
    
  /* Go ahead and flip soft-switch for correct bank */
  if (x % 2 == 0)
    SWITCH_PAGE2=true;
  else
    SWITCH_PAGE1=true;

  x >>= 1; // Divide by 2.

  return screen_addr_row[y]+x;
}

/**
 * @brief Clear specified row, starting at specified column
 * @param x starting Column
 * @param y Row
 * @param n Number of columns
 */
void screen_clear_line(unsigned char x, unsigned char y, unsigned char n)
{
      unsigned char i;

      /* Less efficient, but works. */
      for (i=0;i<n;i++)
	screen_putcxy(x+i,y,' ');

  /* Can someone debug this and see why it fails in certain cases? -Thom */
  /* unsigned char o = _inverse ? 0x20 : 0xA0; */
  /* unsigned char m = n % 2; */

  /* if (!m) */
  /*   { */
  /* n >>= 1; */
  /* x >>= 1; */

  /* if (n > 40) */
  /*   n=40; */
  
  /* if (x > 40) */
  /*   x=40; */

  /* SWITCH_80STORE=true; */
    
  /* SWITCH_PAGE2=true; */
  /* memset((unsigned char *)screen_addr_row[y]+x,o,n-x); */
  /* SWITCH_PAGE1=true; */
  /* memset((unsigned char *)screen_addr_row[y]+x,o,n-x-m); */
  /*   } */
  /* else */
  /*   { */
  /*     unsigned char i; */
  /*     /\* Less efficient, but works. *\/ */
  /*     for (i=0;i<n;i++) */
  /* 	screen_putcxy(x+i,y,' '); */
  /*   } */
}

/**
 * @brief Clear the screen
 */
void screen_clear(void)
{
  unsigned char i;

  for (i=0;i<24;i++)
    screen_clear_line(0,i,80);

}

/**
 * @brief turn on cursor at current position
 */
void screen_cursor_toggle(void)
{
  unsigned char c;
  c=PEEK(screen_addr(_col,_row));
  POKE(screen_addr(_col,_row),c ^ 0x80);
}

/**
 * @brief  Set new cursor_position
 * @param  x column
 * @param  y row
 */
void screen_set_pos(unsigned char x, unsigned char y)
{
  _col = x;
  _row = y;
}

/**
 * @brief  Set new cursor_position (row)
 * @param  x column
 * @param  y row
 */
void screen_set_pos_row(unsigned char y)
{
  _row = y;
}

/**
 * @brief Set character at position
 * @param x Column
 * @param y Row
 * @param c Character to set
 */
void screen_putcxy(unsigned char x, unsigned char y, unsigned char c)
{
  unsigned char o;

  // Prevent screen hole writes.
  if (x>79)
    return;
  else if (y>23)
    return;
  
  if (_inverse)
    {
      if (c>0x3f && c<0x60)
	o=0xC0;
      else
	o=0x00;
    }
  else
    {
      o=0x80;
    }
  
  POKE(screen_addr(x,y),c+o);
}

/**
 * @brief Scroll screen by one line
 */
void screen_scroll_up(void)
{
  unsigned char i=0;

  SWITCH_80STORE=true;

  while (i<24)
    {
      SWITCH_PAGE1=true;
      memcpy((unsigned char *)screen_addr_row[i-1],(unsigned char *)screen_addr_row[i],40);
      SWITCH_PAGE2=true;
      memcpy((unsigned char *)screen_addr_row[i-1],(unsigned char *)screen_addr_row[i],40);
      i++;
    }

  screen_clear_line(0,23,80);
  
  _row=23;
  _col=0;
}

/**
 * @brief Check and adjust cursor wrap
 */
void screen_cursor_wrap(void)
{
  /* Taking advantage of unsigned char */
  if (_col==255)
    _col=79;
  else if (_row==255)
    _row=23;
  else if (_col>79)
    _col=0;
  else if (_row>23)
    _row=0;
}

/**
 * @brief move cursor right N times
 * @param n Number of times to move cursor up
 */
void screen_cursor_right(unsigned char n)
{
  unsigned char i;

  if (n==0)
    n=1;
  
  for (i=0;i<n;i++)
    {
      _col++;
      screen_cursor_wrap();
    }
}

/**
 * @brief move cursor right N times
 * @param n Number of times to move cursor up
 */
void screen_cursor_left(unsigned char n)
{
  unsigned char i;

  if (n==0)
    n=1;
  
  for (i=0;i<n;i++)
    {
      _col--;
      screen_cursor_wrap();
    }
}

/**
 * @brief move cursor up N times
 * @param n Number of times to move cursor up
 */
void screen_cursor_up(unsigned char n)
{
  unsigned char i;

  if (n==0)
    n=1;
  
  for (i=0;i<n;i++)
    {
      _row--;
      screen_cursor_wrap();
    }
}

/**
 * @brief move cursor down N times
 * @param n Number of times to move cursor down
 */
void screen_cursor_down(unsigned char n)
{
  unsigned char i;

  if (n==0)
    n=1;
  
  for (i=0;i<n;i++)
    {
      _row++;
      screen_cursor_wrap();
    }
}

/**
 * @brief Set inverse characters
 */
void screen_inverse(void)
{
  _inverse=true;
}

/**
 * @brief Set normal characters
 */
void screen_normal(void)
{
  _inverse=false;
}

/**
 * @brief Put character at current position, advance cursor, scroll if needed
 * @param c Character to place.
 */
void screen_putc(unsigned char c)
{
  screen_putcxy(_col,_row,c);

  if (_col==80)
    {
      _col=0;
      
      if (_row==23)
	{
	  screen_scroll_up();
	  return;
	}
      else
	_row++;
    }
  else
    {
      _col++;
    }
  
}

/**
 * @brief Send string s to screen
 * @param s C-style string
 */
void screen_puts(const char *c)
{
  while (*c)
    screen_putc(*c++);
}

/**
 * @brief  Insert a line above cursor row
 * @param  n Number of lines 
 */
void screen_insert_line(unsigned char n)
{
  unsigned char b = (_inverse ? 0x20 : 0xA0);
  unsigned char r=23;
  unsigned char j;
  
  if (n==0)
    n=1;
  
  SWITCH_80STORE=true;

  for (j=0;j<n;j++)
    {
      while (r>_row)
	{
	  SWITCH_PAGE1=true;
	  memcpy((unsigned char *)screen_addr_row[r],(unsigned char *)screen_addr_row[r-1],40);
	  SWITCH_PAGE2=true;
	  memcpy((unsigned char *)screen_addr_row[r],(unsigned char *)screen_addr_row[r-1],40);
	  r--;
	}
      
      screen_clear_line(0,_row,80);
    }
}

/**
 * @brief Delete line
 */
void screen_delete_line(unsigned char n)
{
  unsigned char i=_row+1;
  unsigned char j;

  if (n==0)
    n=1;
  
  SWITCH_80STORE=true;

  for (j=0;j<n;j++)
    {
      while (i<24)
	{
	  SWITCH_PAGE1=true;
	  memcpy((unsigned char *)screen_addr_row[i-1],(unsigned char *)screen_addr_row[i],40);
	  SWITCH_PAGE2=true;
	  memcpy((unsigned char *)screen_addr_row[i-1],(unsigned char *)screen_addr_row[i],40);
	  i++;
	}
      screen_clear_line(0,23,80);
    }
}

/**
 * @brief Initialize 80 column text screen
 */
void screen_init(void)
{
  SWITCH_80COL=SWITCH_80STORE=SWITCH_ALTCHAR=true;
  screen_clear();
  
}
