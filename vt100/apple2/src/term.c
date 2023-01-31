/**
 * @brief   Terminal for Apple ][
 * @verbose Implements functions in vt100.c
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2.h>
#include "vt100.h"
#include "screen.h"

/* externs from screen.c */
extern unsigned char _row;
extern unsigned char _col;

void cursor_toggle(void)
{
  screen_cursor_toggle();
}

void use(void)
{
  screen_use();
}

void unuse(void)
{
  screen_unuse();
}

void bel(void)
{
  screen_bell();
}

/* response injection */
void sendback(char c)
{
  (void)c; // Temporary for now.
}

/* Character commands */
void bs(void)
{
  screen_bs();
}

void tab(void)
{
  screen_tab();
}

void lf(void)
{
  screen_lf();
}

void cr(void)
{
  screen_cr();
}

void xoff(void)
{
  // not implemented
}

void xon(void)
{
  // not implemented
}

void toscreen(char c)
{
  if (c > 0x1F)
    screen_putc(c);
}


/* Cursor control */
void cuu(void)
{
  screen_cursor_up(1);
}

void cud(void)
{
  screen_cursor_down(1);
}

void cur(void)
{
  screen_cursor_right(1);
}

void cul(void)
{
  screen_cursor_left(1);
}

void cup(unsigned char r, unsigned char c)
{
  screen_set_pos(c-1,r-1);
}

void cupr(unsigned char r)
{
  screen_set_pos_row(r);
}

void ind(void)
{
  lf();
}

void ri(void)
{
  if (_row>0)
    _row--;
  else
    screen_scroll_up();
}

void nel(void)
{
  cr();
  lf();
}

void whereami(unsigned char *r, unsigned char *c)
{
  /* These come straight from screen.c */
  *r=_row;
  *c=_col;
}


/* Tab control */
void hts(void)
{
}

void clear_current_tab_stop(void)
{
}

void clear_all_tab_stops(void)
{
}


/* Character attributes */
void reset_attributes(void)
{
  screen_normal();
}

void bold(void)
{
  screen_inverse();
}

void lo(void)
{
  screen_normal();
}

void underline(void)
{
  screen_inverse();
}

void blink(void)
{
  screen_inverse();
}

void inverse(void)
{
  screen_inverse();
}

void invisible(void)
{
}


/* Clear commands */
void clear_cursor_to_end(void)
{
  screen_clear_cursor_to_end();
}

void clear_beginning_to_cursor(void)
{
  screen_clear_beg_to_cursor();
}

void clear_entire_screen(void)
{
  screen_clear();
}

void clear_to_end_of_line(void)
{
  screen_clear_to_end_of_line();
}

void clear_line_until_cursor(void)
{
  screen_clear_beg_to_cursor();
}

void clear_entire_line(void)
{
  screen_clear_current_line();
}


/* Media Control commands */
void printer_off(void)
{
}

void printer_on(void)
{
}

/* Insert/delete */
void insert_line(unsigned char n)
{
  screen_insert_line(n);
}
