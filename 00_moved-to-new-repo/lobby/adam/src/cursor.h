/**
 * FujiNet Config for Adam
 * 
 * Cursor routines
 */

#ifndef CURSOR_H
#define CURSOR_H

#include <stdbool.h>

void cursor(bool t);
void cursor_pos(unsigned char x, unsigned char y);

void selector(unsigned char t);
void selector_pos(unsigned char x, unsigned char y);


#endif /* CURSOR_H */
