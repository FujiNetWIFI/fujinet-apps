#ifdef __PLUS4__

#include <plus4.h>
#include <stdint.h>
#include <stdbool.h>

bool always_render_full_cards=true;
unsigned char colorMode = 0;

void enableDoubleBuffer()
{
}

void disableDoubleBuffer()
{
}

void drawBuffer()
{
}

void resetScreen()
{
}

void clearStatusBar()
{
}

void drawStatusTextAt(unsigned char x, char* s)
{
}

void drawStatusText(char* s)
{
}

void drawStatusTimer()
{
}

void drawText(unsigned char x, unsigned char y, const char* s)
{
}

void drawCard(unsigned char x, unsigned char y, unsigned char partial, const char* s, unsigned char isHidden)
{
}

void drawChip(unsigned char x, unsigned char y)
{
}

void drawBlank(unsigned char x, unsigned char y)
{
}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
}

void hideLine(unsigned char x, unsigned char y, unsigned char w)
{
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
}

void drawBorder()
{
}

void drawLogo()
{
}

void initGraphics()
{
}

void resetGraphics()
{
}

void waitvsync()
{
}

uint8_t cycleNextColor()
{
}

void setColorMode()
{
}

#endif /* __PLUS4__ */
