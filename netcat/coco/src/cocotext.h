#ifndef COCOTEXT_H
#define COCOTEXT_H

#include "fujinet-fuji.h"
#include "hirestxt.h"

#define isprint(c) (c>=0x20 && c<=0x8E)

#define ARROW_UP 0x5E
#define ARROW_DOWN 0x0A
#define ARROW_LEFT 0x08
#define ARROW_RIGHT 0x09
#define ENTER 0x0D
#define BREAK 0x03

#define COLORSET_WHITE 1
#define COLORSET_GREEN 0
#define SCREEN_WHITE 5
#define SCREEN_GREEN 1

#define gotox(x) moveCursor(x, getCursorRow())

void hirestxt_init(void);
void hirestxt_close(void);
void switch_colorset(void);
void gotoxy(byte x, byte y);
void cursor(bool onoff);
void clear_screen(byte color);
void set_text_width(byte screen_width);
char *screen_upper(const char *s);
char *screen_lower(const char *s);
byte cgetc();
void get_line(char *buf, uint8_t max_len);


extern bool hirestxt_mode;
extern byte textMode;
extern bool cursor_on;
extern byte colorset;
extern byte bgcolor;

#endif // COCOTEXT_H