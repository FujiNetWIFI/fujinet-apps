/*
 Platform specific graphics commands
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define PARTIAL_LEFT 1
#define PARTIAL_RIGHT 2
#define FULL_CARD 3

#include <stdint.h>
#include <stdbool.h>
#include "../misc.h"


// Call to clear the screen to an empty table
void resetScreen();

void drawText(unsigned char x, unsigned char y, char* s);
void drawTextAlt(unsigned char x, unsigned char y, char* s);
void drawTextCursor(unsigned char x, unsigned char y);
void drawTextVert(unsigned char x, unsigned char y, char* s);

void drawDie(unsigned char x, unsigned char y, const char* s, bool isSelected);
void drawChip(unsigned char x, unsigned char y);
void drawBlank(unsigned char x, unsigned char y);
void drawSpace(unsigned char x, unsigned char y, unsigned char w);
void drawLine(unsigned char x, unsigned char y, unsigned char w);
void hideLine(unsigned char x, unsigned char y, unsigned char w);
void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void drawBorder();
void drawBoard();

void initGraphics();
void waitvsync();

uint8_t cycleNextColor();
void setColorMode();
extern unsigned char colorMode;

#endif /* GRAPHICS_H */