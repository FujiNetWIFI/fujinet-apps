/*
 Platform specific graphics commands
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define PARTIAL_LEFT 1
#define PARTIAL_RIGHT 2
#define FULL_CARD 3

// Include platform specific defines before the input include
#include "../c64/vars.h"
#include "../apple2/vars.h"
#include "../adam/vars.h"
#include "../coco/vars.h"
#include "../msdos/vars.h"
#include "../plus4/vars.h"
#include "../lynx/vars.h"


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
void drawStatusTextAt(unsigned char x, const char* s);
void drawStatusText(const char* s);
void drawStatusTimer();


// For optional HI-RES Y enabled platforms
void drawTextAt(unsigned char x, unsigned char y, const char* s);

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
void resetGraphics();
void waitvsync();

uint8_t cycleNextColor();
void setColorMode();
extern unsigned char colorMode;

// For platforms that do not have keyboards, or may want to use different keys
// platformStatusKeyLegend is the status line displayed during select table screen
// platformStatusMenuKeys is the in-game menu
#ifdef USE_PLATFORM_SPECIFIC_KEYS
void platformStatusKeyLegend();
void platformMenuKeys();
#endif

#endif /* GRAPHICS_H */
