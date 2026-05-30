/**
 * @brief CoCo 3 80-column screen driver API.
 *
 * All drawing operations update the RAM shadow buffer. screen_flush() copies
 * dirty shadow rows to the hardware 40/80-column text screen.
 */

#ifndef SCREEN_H
#define SCREEN_H

/* ---- lifecycle / hardware display ---- */

void screen_init(void);
void screen_shutdown(void);
void screen_flush(void);
void screen_redraw(void);
void screen_palette(unsigned char composite);

/* ---- character output ---- */

void screen_putc(unsigned char c);
void screen_puts_run(const unsigned char *buf, unsigned int len);
void screen_bs(void);
void screen_lf(void);
void screen_cr(void);
void screen_tab(void);

/* ---- cursor and saved state ---- */

void screen_set_pos(unsigned char x, unsigned char y);
void screen_get_pos(unsigned char *row, unsigned char *col);
void screen_cursor_right(unsigned char n);
void screen_cursor_left(unsigned char n);
void screen_cursor_up(unsigned char n);
void screen_cursor_down(unsigned char n);
void screen_show_cursor(unsigned char on);
void screen_save_cursor(void);
void screen_restore_cursor(void);

/* ---- VT100 modes ---- */

void screen_set_region(unsigned char top, unsigned char bottom);
void screen_set_autowrap(unsigned char on);
void screen_set_origin(unsigned char on);
void screen_set_reverse(unsigned char on);
void screen_set_appcursor(unsigned char on);
unsigned char screen_appcursor(void);

/* ---- clearing, scrolling, insert/delete ---- */

void screen_clear(void);
void screen_clear_cursor_to_end(void);
void screen_clear_beg_to_cursor(void);
void screen_clear_to_end_of_line(void);
void screen_clear_line_to_cursor(void);
void screen_clear_current_line(void);
void screen_scroll_up(void);
void screen_scroll_down(void);
void screen_ri(void);
void screen_insert_line(unsigned char n);
void screen_delete_line(unsigned char n);
void screen_decaln(void);

/* ---- tab stops ---- */

void screen_set_tab(void);
void screen_clear_tab(void);
void screen_clear_all_tabs(void);

/* ---- attributes ---- */

void screen_attr_reset(void);
void screen_attr_underline(void);
void screen_attr_blink(void);
void screen_attr_inverse(void);
void screen_attr_invisible(void);
void screen_set_fg(unsigned char c);
void screen_set_bg(unsigned char c);

/* ---- transient direct-to-hardware overlay ---- */

void screen_overlay_clear(void);
void screen_overlay_line(unsigned char row, const char *s);

#endif /* SCREEN_H */
