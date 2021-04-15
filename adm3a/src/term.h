/**
 * Terminal implementation for ADM-3A
 */

#ifndef TERM_H
#define TERM_H

#include <stdbool.h>

/**
 * Terminal processing entry-point
 * @param buf - The buffer to interpret
 * @param len - number of bytes to interpret
 * @return true if error.
 */
bool term(char* buf, unsigned short len);

#endif /* TERM_H */
