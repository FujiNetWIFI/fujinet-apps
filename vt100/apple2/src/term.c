/**
 * @brief   Terminal for Apple ][
 * @verbose Implements functions in vt100.c
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2enh.h>
#include <conio.h>
#include "vt100.h"

unsigned char tabs[8]={8,16,24,32,40,48,56,64,72};

/**
 * @brief Scroll the window down by one line.
 */
void scroll_down()
{
}

/* response injection */
void sendback(char c)
{
}


/* Character commands */
void bs(void)
{
  cputc(BS);
}

void tab(void)
{
  cputc(TAB);
}

void lf(void)
{
  cputc(LF);
}

void cr(void)
{
  cputc(CR);
}

void xoff(void)
{
  
}

void xon(void)
{
}

void toscreen(char c)
{
  cputc(c);
}


/* Cursor control */
void cuu(void)
{
}

void cud(void)
{
}

void cur(void)
{
}

void cul(void)
{
}

void cup(unsigned char r, unsigned char c)
{
  gotoxy(r,c);
}

void ind(void)
{
}

void ri(void)
{
}

void nel(void)
{
}

void whereami(unsigned char *r, unsigned char *c)
{
  *r=wherey();
  *c=wherex();
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
}

void bold(void)
{
}

void lo(void)
{
}

void underline(void)
{
}

void blink(void)
{
}

void inverse(void)
{
}

void invisible(void)
{
}


/* Clear commands */
void clear_cursor_to_end(void)
{
}

void clear_beginning_to_cursor(void)
{
}

void clear_entire_screen(void)
{
}

void clear_to_end_of_line(void)
{
}

void clear_line_until_cursor(void)
{
}

void clear_entire_line(void)
{
}


/* Media Control commands */
void printer_off(void)
{
}

void printer_on(void)
{
}

