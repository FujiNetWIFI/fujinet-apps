/**
 * Proportional Font text output library for Atari
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#ifndef PFONT_H
#define PFONT_H

/**
 * @brief set cursor to X,Y
 * @param x new X position (0-319)
 * @param y new Y position (0-191)
 */
void pgotoxy(unsigned short x, unsigned char y);

/**
 * @brief Output a proportional font character at cursor
 * @param c Character to output
 */
unsigned char pputc(const char c);

/**
 * @brief Write string to screen using proportional font, at cursor.
 * @param c NULL terminated string to display
 */
unsigned char pputs(const char *c);

#endif /* PFONT_H */
