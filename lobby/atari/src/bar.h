/**
 * Bar routines
 */

#ifndef BAR_H
#define BAR_H

/**
 * Clear the currently displayed bar from screen
 * @param old true = old position, false = current position
 */
void bar_clear(void);

/**
 * Show bar at Y position
 */
void bar_show(unsigned char y);

/**
 * Set Bar Color
 * @param f foreground color 0-15
 * @param b background color 0-15
 */
void bar_set_color(unsigned char c);

#endif /* BAR_H */
