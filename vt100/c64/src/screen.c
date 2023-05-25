/**
 * @brief   Screen routines for VT100
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <c64.h>
#include <conio.h>

static unsigned char col = 0xFF,row = 0xFF;

void screen_cursor_toggle(void)
{
  char c;

}

void screen_gotoxy(unsigned char _col, unsigned char _row)
{
  if (col == 0xFF)
    return;
    
  screen_cursor_toggle();
  
  col = _col;
  row = _row;

  screen_cursor_toggle();
}

void screen_putc(unsigned char c)
{
  
}
  
void screen_clrscr(void)
{
  clrscr();
  screen_gotoxy(0,0);
}

void main(void)
{
  screen_clrscr();
  
  cgetc();
}
