#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <stdint.h>

#define ATTRIBUTE_HEADER   0x70
#define ATTRIBUTE_BORDER   0x1F
#define ATTRIBUTE_NORMAL   0x17
#define ATTRIBUTE_BOLD     0x1F
#define ATTRIBUTE_SELECTED 0x70
#define ATTRIBUTE_STATUS   0x70

#define SCREEN_ROWS 25

extern unsigned char screen_cols;
extern unsigned char far *video;

void screen_init(void);
void screen_end(void);
void screen_clear(void);
void screen_clear_line(unsigned char y);
void screen_fill_line(unsigned char y, unsigned char a);
void screen_putc(unsigned char x, unsigned char y, unsigned char a, char c);
void screen_puts(unsigned char x, unsigned char y, unsigned char a, const char *s);
void screen_puts_center(unsigned char y, unsigned char a, const char *s);
void gotoxy(unsigned char x, unsigned char y);
void cursor(bool on);
unsigned char cgetc(void);
bool get_line(char *buf, uint8_t max_len);
void hd_bar(unsigned char y, const char *text, unsigned char attr);
void show_fetching_msg(bool clear);

#endif /* SCREEN_H */
