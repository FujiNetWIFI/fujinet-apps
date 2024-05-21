/**
 * @brief   Apple2 Text I/O
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Uses Apple2 ROM routines. Thanks to Oliver Schmidt.
 */

#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

/**
 * @brief called to initialize 80 column using PR#3
 */
void init80(void);

/**
 * @brief return if 80 column detected
 */
bool has80column(void);

/**
 * @brief Output a character using COUT
 * @param c Character to output
 */
void __fastcall__ outc(char c);

/**
 * @brief send character string using outc()
 * @param s The NULL-terminated string to send through outc()
 */
void outs(const char *s);

/**
 * @brief place cursor character at position via inverted blank 
 */
void place_cursor(void);

/**
 * @brief remove cursor character at position via BS + blank + BS
 */
void remove_cursor(void);

#endif /* TEXT_H */
