#ifndef BAR_H
#define BAR_H

#include <stdbool.h>
#include <stdint.h>

void bar_set(unsigned char y, unsigned char c, unsigned char m, unsigned char i);
void bar_clear(bool old);
void bar_draw(int y, bool clear);
uint8_t bar_get(void);
void bar_up(void);
void bar_down(void);
void bar_update(void);

#endif /* BAR_H */
