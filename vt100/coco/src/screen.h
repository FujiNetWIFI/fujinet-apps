/**
 * 80 column routines
 * 
 * Screen routines
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef SCREEN_H
#define SCREEN_H

/**
 * @brief sound bell
 */
void screen_bell(void);

/**
 * @brief do backspace
 */
void screen_bs(void);

/**
 * @brief do linefeeds
 */
void screen_lf(void);

/**
 * @brief do carriage return
 */
void screen_cr(void);

/**
 * @brief go to next TAB
 */
void screen_tab(void);

/**
 * @brief set a tab stop at the cursor (HTS)
 */
void screen_set_tab(void);

/**
 * @brief clear the tab stop at the cursor (TBC 0)
 */
void screen_clear_tab(void);

/**
 * @brief clear all tab stops (TBC 3)
 */
void screen_clear_all_tabs(void);

/**
 * @brief clear from current pos to end of screen
 */
void screen_clear_cursor_to_end(void);

/**
 * @brief clear from beginning of screen to cursor (ED mode 1)
 */
void screen_clear_beg_to_cursor(void);

/**
 * @brief clear current line from start to cursor (EL mode 1)
 */
void screen_clear_line_to_cursor(void);

/**
 * @brief Clear current line
 */
void screen_clear_current_line(void);

/**
 * @brief clear to end of line
 */
void screen_clear_to_end_of_line(void);

/**
 * @brief move cursor right N times
 * @param n Number of times to move cursor right
 */
void screen_cursor_right(unsigned char n);

/**
 * @brief move cursor left N times
 * @param n Number of times to move cursor left
 */
void screen_cursor_left(unsigned char n);

/**
 * @brief move cursor up N times
 * @param n Number of times to move cursor up
 */
void screen_cursor_up(unsigned char n);

/**
 * @brief move cursor down N times
 * @param n Number of times to move cursor down
 */
void screen_cursor_down(unsigned char n);

/**
 * @brief Scroll screen by one line
 */
void screen_scroll_up(void);

/**
 * @brief Clear specified row, starting at specified column
 * @param x Column
 * @param y Row
 * @param n number of characters
 */
void screen_clear_line(unsigned char x, unsigned char y, unsigned char n);

/**
 * @brief Clear the screen
 */
void screen_clear(void);

/**
 * @brief Initialize 80 column text screen
 */
void screen_init(void);

/**
 * @brief Put character at current position, advance cursor, scroll if needed
 * @param c Character to place.
 */
void screen_putc(unsigned char c);

/**
 * @brief  Set new cursor_position
 * @param  x column
 * @param  y row
 */
void screen_set_pos(unsigned char x, unsigned char y);

/**
 * @brief  Insert a line above cursor row
 * @param  n Number of lines 
 */
void screen_insert_line(unsigned char n);

/**
 * @brief  Delete a line
 * @param  n Number of lines
 */
void screen_delete_line(unsigned char n);

/* ---- CoCo 3 additions ---- */

/**
 * @brief  Blit the RAM shadow to the hardware 80-col text screen.
 *         All other screen_* calls update the shadow only; this makes them
 *         visible. Draws a block cursor at the current position.
 */
void screen_flush(void);

/**
 * @brief  Scroll the screen down by one line (reverse index), blank top row.
 */
void screen_scroll_down(void);

/**
 * @brief  Set the scrolling region (DECSTBM). 1-based rows; 0 = full screen.
 */
void screen_set_region(unsigned char top, unsigned char bottom);

/**
 * @brief  Reverse index: up one line, scroll region down at the top margin.
 */
void screen_ri(void);

/**
 * @brief  DECAWM: enable (1) / disable (0) autowrap at the right margin.
 */
void screen_set_autowrap(unsigned char on);

/**
 * @brief  DECOM: enable (1) / disable (0) origin mode (cursor relative to region).
 */
void screen_set_origin(unsigned char on);

/**
 * @brief  DECSCNM: enable (1) / disable (0) reverse video for the whole screen.
 */
void screen_set_reverse(unsigned char on);

/**
 * @brief  DECCKM application cursor key mode: set (1) / reset (0) and query.
 *         Selects the arrow-key form the keyboard sends (ESC O x vs ESC [ x).
 */
void screen_set_appcursor(unsigned char on);
unsigned char screen_appcursor(void);

/**
 * @brief  Enable/disable drawing the block cursor on screen_flush().
 */
void screen_show_cursor(unsigned char on);

/**
 * @brief  Blank the hardware screen directly (overlay; does not touch shadow).
 */
void screen_overlay_clear(void);

/**
 * @brief  Draw a string at a hardware row directly (overlay; not the shadow).
 */
void screen_overlay_line(unsigned char row, const char *s);

/**
 * @brief  Re-blit the whole shadow, restoring the session under an overlay.
 */
void screen_redraw(void);

/**
 * @brief  Save / restore cursor position and attribute (DECSC / DECRC).
 */
void screen_save_cursor(void);
void screen_restore_cursor(void);

/**
 * @brief  DECALN screen test: fill the screen with 'E', cursor home.
 */
void screen_decaln(void);

/**
 * @brief  Bulk-write a run of printable bytes at the cursor (fast path).
 *         Caller guarantees the decoder is in CHAR state and bytes are printable.
 */
void screen_puts_run(const unsigned char *buf, unsigned int len);

/* Character attributes (CoCo 3 attribute byte: blink<<7|underline<<6|fg<<3|bg) */
void screen_attr_reset(void);
void screen_attr_bold(void);
void screen_attr_underline(void);
void screen_attr_blink(void);
void screen_attr_inverse(void);
void screen_attr_invisible(void);
void screen_set_fg(unsigned char c);     /* 0-7 ANSI colour index */
void screen_set_bg(unsigned char c);

/**
 * @brief  Select monitor type (0 = RGB, 1 = composite) and load the palette.
 */
void screen_palette(unsigned char composite);

#endif /* SCREEN_H */
