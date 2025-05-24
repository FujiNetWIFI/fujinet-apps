/*
 Platform specific graphics commands
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define PARTIAL_LEFT 1
#define PARTIAL_RIGHT 2
#define FULL_CARD 3

#ifdef _CMOC_VERSION_
#include "../coco/coco_bool.h"
#else
#include <stdint.h>
#include <stdbool.h>
#endif /* _CMOC_VERSION_  */
#include "../misc.h"

extern bool always_render_full_cards;

void enableDoubleBuffer();
void disableDoubleBuffer();
void drawBuffer();

// Call to clear the screen to an empty table
void resetScreen();

void clearStatusBar();
void drawStatusTextAt(unsigned char x, char* s);
void drawStatusText(char* s);
void drawStatusTimer();

void drawText(unsigned char x, unsigned char y, const char* s);
void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden);
void drawChip(unsigned char x, unsigned char y);
void drawBlank(unsigned char x, unsigned char y);
void drawLine(unsigned char x, unsigned char y, unsigned char w);
void hideLine(unsigned char x, unsigned char y, unsigned char w);
void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void drawBorder();
void drawLogo();

void initGraphics();
void waitvsync();

uint8_t cycleNextColor();
void setColorMode();
extern unsigned char colorMode;

#endif /* GRAPHICS_H */
