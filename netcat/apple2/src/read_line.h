/**
 * @brief   Line Input routine
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Gosh I love writing these things, really, I do... #@(%#@)_%@
 */

#ifndef READ_LINE_H
#define READ_LINE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief read an input line.
 * @param s The output buffer. Must be at least len bytes.
 * @param len Allowable # of characters (incl. \0).
 * @param password Obscure character output.
 */
void read_line(char *s, uint16_t len, bool password);

#endif /* READ_LINE_H */
