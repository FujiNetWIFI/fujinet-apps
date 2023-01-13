/**
 * ADM-3A terminal state machine
 */

#ifndef TERM_H
#define TERM_H

#include <stdbool.h>

/**
 * Terminal emulator entrypoint
 * @param buffer to translate
 * @param len length of buffer to translate
 * @return true on error
 */
bool term(char* buf, unsigned short len);

#endif /* TERM_H */
