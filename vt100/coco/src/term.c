/**
 * @brief vt100 engine callbacks for the CoCo 3.
 *
 * The engine (vt100.c) calls these; they drive the screen shadow. Attribute
 * callbacks use the CoCo 3 hardware attribute byte (real underline/blink/
 * inverse), unlike the Apple II port which collapses everything to inverse.
 */

#include "vt100.h"
#include "screen.h"

/* cursor position lives in screen.c */
extern unsigned char _row;
extern unsigned char _col;

/* set by main.c so DSR/cursor-report replies can be sent back over the wire */
void (*term_sendback)(char c) = 0;

void bel(void) { screen_bell(); }

void sendback(char c)
{
    if (term_sendback)
        term_sendback(c);
}

/* character commands */
void bs(void)  { screen_bs(); }
void tab(void) { screen_tab(); }
void lf(void)  { screen_lf(); }
void cr(void)  { screen_cr(); }
void xoff(void) { }
void xon(void)  { }

void toscreen(char c)
{
    if ((unsigned char) c > 0x1F)
        screen_putc((unsigned char) c);
}

/* cursor control */
void cuu(void) { screen_cursor_up(1); }
void cud(void) { screen_cursor_down(1); }
void cur(void) { screen_cursor_right(1); }
void cul(void) { screen_cursor_left(1); }

void cup(unsigned char r, unsigned char c)
{
    if (!r) r = 1;                 /* ESC[H with no params -> home (1,1) */
    if (!c) c = 1;
    screen_set_pos(c - 1, r - 1);
}

void set_region(unsigned char top, unsigned char bottom) { screen_set_region(top, bottom); }

void ind(void) { lf(); }

void ri(void) { screen_ri(); }

void nel(void) { cr(); lf(); }

void whereami(unsigned char *r, unsigned char *c)
{
    *r = _row;
    *c = _col;
}

void save_cursor(void)    { screen_save_cursor(); }
void restore_cursor(void) { screen_restore_cursor(); }
void decaln(void)         { screen_decaln(); }

void set_autowrap(unsigned char on) { screen_set_autowrap(on); }
void set_origin(unsigned char on)   { screen_set_origin(on); }
void set_reverse(unsigned char on)  { screen_set_reverse(on); }
void set_appcursor(unsigned char on){ screen_set_appcursor(on); }

/* insert / delete */
void insert_line(unsigned char n) { screen_insert_line(n); }
void delete_line(unsigned char n) { screen_delete_line(n); }

/* tab control */
void hts(void)                   { screen_set_tab(); }
void clear_current_tab_stop(void){ screen_clear_tab(); }
void clear_all_tab_stops(void)   { screen_clear_all_tabs(); }

/* character attributes */
void reset_attributes(void) { screen_attr_reset(); }
void bold(void)             { screen_attr_bold(); }
void lo(void)               { screen_attr_reset(); }
void underline(void)        { screen_attr_underline(); }
void blink(void)            { screen_attr_blink(); }
void inverse(void)          { screen_attr_inverse(); }
void invisible(void)        { screen_attr_invisible(); }

/* ANSI colours (SGR 30-37/40-47, 90-97/100-107) -> CoCo attribute colours */
void fg_color(unsigned char c) { screen_set_fg(c); }
void bg_color(unsigned char c) { screen_set_bg(c); }
void fg_default(void)          { screen_set_fg(7); }   /* white */
void bg_default(void)          { screen_set_bg(0); }   /* black */

/* clear commands */
void clear_cursor_to_end(void)      { screen_clear_cursor_to_end(); }    /* ED 0 */
void clear_beginning_to_cursor(void){ screen_clear_beg_to_cursor(); }    /* ED 1 (display) */
void clear_entire_screen(void)      { screen_clear(); }                  /* ED 2 */
void clear_to_end_of_line(void)     { screen_clear_to_end_of_line(); }   /* EL 0 */
void clear_line_until_cursor(void)  { screen_clear_line_to_cursor(); }   /* EL 1 (line) */
void clear_entire_line(void)        { screen_clear_current_line(); }     /* EL 2 */

/* media control */
void printer_off(void) { }
void printer_on(void) { }
