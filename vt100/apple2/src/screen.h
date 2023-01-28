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
 * @brief swap in screen
 */
void screen_use(void);

/**
 * @brief swap out screen
 */
void screen_unuse(void);

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
 * @brief clear from current pos to end of screen
 */
void screen_clear_cursor_to_end(void);

/**
 * @brief clear from beginning to cursor
 */
void screen_clear_beg_to_cursor(void);

/**
 * @brief Clear current line
 */
void screen_clear_current_line(void);

/**
 * @brief clear to end of line
 */
void screen_clear_to_end_of_line(void);

/**
 * @brief  Get screen address and set page softswitch
 * @param  x Column
 * @param  y Row
 * @return Destination address.
 */
unsigned short screen_addr(unsigned char x, unsigned char y);

/**
 * @brief turn on cursor at current position
 */
void screen_cursor_toggle(void);

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
 * @brief Update cursor position
 */
void screen_update_cursor_position(void);

/**
 * @brief Initialize 80 column text screen
 */
void screen_init(void);

/**
 * @brief The test function
 */
void screen_test(void);

/**
 * @brief Set inverse characters
 */
void screen_inverse(void);

/**
 * @brief Set normal characters
 */
void screen_normal(void);

/**
 * @brief Set character at position
 * @param x Column
 * @param y Row
 * @param c Character to set
 */
void screen_putcxy(unsigned char x, unsigned char y, unsigned char c);

/**
 * @brief Put character at current position, advance cursor, scroll if needed
 * @param c Character to place.
 */
void screen_putc(unsigned char c);

/**
 * @brief Send string s to screen
 * @param s C-style string
 */
void screen_puts(const char *c);

/**
 * @brief  Set new cursor_position
 * @param  x column
 * @param  y row
 */
void screen_set_pos(unsigned char x, unsigned char y);

#endif /* SCREEN_H */
