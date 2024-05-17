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
 * @brief place cursor character at position via COUT 
 */
void __fastcall__ place_cursor(void);

/**
 * @brief called to initialize 80 column using PR#3
 */
void init80(void);

/**
 * @brief Check if 80 column
 */
void check80column(void);

/**
 * @brief return if 80 column detected
 */
bool is80column(void);

/**
 * @brief Get character into return
 * @return key pressed
 */
unsigned char __fastcall__ inc(void);

/**
 * @brief Output a character using COUT
 * @param c Character to output
 */
void outc(char c);

/**
 * @brief send character string using outc()
 * @param s The NULL-terminated string to send through outc()
 */
void outs(const char *s);

#endif /* TEXT_H */
