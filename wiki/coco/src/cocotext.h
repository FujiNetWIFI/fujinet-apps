#ifndef COCOTEXT_H
#define COCOTEXT_H

#include "fujinet-fuji.h"
#include "hirestxt.h"

#define WIDTH_CANCEL 99

#define gotox(x) moveCursor(x, getCursorRow())

byte text_width_menu(void);
void hirestxt_init(void);
void hirestxt_close(void);
void switch_colorset(void);
void gotoxy(byte x, byte y);
void cursor(unsigned char onoff);
void clear_screen(byte color);
void select_screen_width(void);
void set_text_width(byte screen_width);
char *screen_upper(const char *s);
char *screen_lower(const char *s);
void reverse(bool onoff);
void print_reverse(byte x, byte y, const char *text, bool lowercase_only);
void show_fetching_msg(bool clear);
byte cgetc_cursor();
bool get_line(char *buf, uint8_t max_len);


extern bool hirestxt_mode;

#endif // COCOTEXT_H