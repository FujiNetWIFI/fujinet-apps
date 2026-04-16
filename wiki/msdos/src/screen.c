#include <dos.h>
#include <i86.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "screen.h"
#include "globals.h"

unsigned char far *video = NULL;
unsigned char screen_cols = 80;

static unsigned char far *screen_get_video_segment_address(void)
{
    static union REGS regs;

    regs.h.ah = 0x0F;
    int86(0x10, &regs, &regs);

    if (regs.h.al <= 0x07)
    {
        unsigned short equip = *(unsigned short far *)MK_FP(0x0040, 0x0010);
        if ((equip & 0x30) == 0x30)
            return (unsigned char far *)MK_FP(0xB000, 0x0000);
        else
            return (unsigned char far *)MK_FP(0xB800, 0x0000);
    }

    return (unsigned char far *)MK_FP(0xB800, 0x0000);
}

void screen_init(void)
{
    static union REGS r;

    r.h.ah = 0x0F;
    int86(0x10, &r, &r);
    screen_cols = r.h.ah;

    video = screen_get_video_segment_address();
    screen_clear();
    cursor(false);
}

void screen_end(void)
{
    screen_clear();
    cursor(true);
    gotoxy(0, 0);
}

void screen_clear(void)
{
    unsigned short i;
    unsigned short total = (unsigned short)screen_cols * SCREEN_ROWS * 2;
    for (i = 0; i < total; i += 2)
    {
        video[i]     = 0x20;
        video[i + 1] = ATTRIBUTE_NORMAL;
    }
}

void screen_clear_line(unsigned char y)
{
    static union REGS r;

    r.h.ah = 0x02;
    r.h.bh = 0;
    r.h.dh = y;
    r.h.dl = 0;
    int86(0x10, &r, &r);

    r.h.ah = 0x09;
    r.h.al = 0x20;
    r.h.bh = 0;
    r.h.bl = ATTRIBUTE_NORMAL;
    r.x.cx = screen_cols;
    int86(0x10, &r, &r);
}

void screen_fill_line(unsigned char y, unsigned char a)
{
    static union REGS r;
    r.h.ah = 0x02; r.h.bh = 0; r.h.dh = y; r.h.dl = 0;
    int86(0x10, &r, &r);
    r.h.ah = 0x09; r.h.al = 0x20; r.h.bh = 0; r.h.bl = a; r.x.cx = screen_cols;
    int86(0x10, &r, &r);
}

void screen_putc(unsigned char x, unsigned char y, unsigned char a, char c)
{
    static union REGS r;

    r.h.ah = 0x02;
    r.h.bh = 0;
    r.h.dh = y;
    r.h.dl = x;
    int86(0x10, &r, &r);

    r.h.ah = 0x09;
    r.h.al = (unsigned char)c;
    r.h.bh = 0;
    r.h.bl = a;
    r.x.cx = 1;
    int86(0x10, &r, &r);
}

void screen_puts(unsigned char x, unsigned char y, unsigned char a, const char *s)
{
    char c;
    while ((c = *s++) != 0)
    {
        if (x >= screen_cols)
        {
            x = 0;
            y++;
        }
        screen_putc(x++, y, a, c);
    }
}

void screen_puts_center(unsigned char y, unsigned char a, const char *s)
{
    unsigned char x = (screen_cols >> 1) - ((unsigned char)strlen(s) >> 1);
    screen_puts(x, y, a, s);
}

void gotoxy(unsigned char x, unsigned char y)
{
    static union REGS r;
    r.h.ah = 0x02;
    r.h.bh = 0;
    r.h.dh = y;
    r.h.dl = x;
    int86(0x10, &r, &r);
}

void cursor(bool on)
{
    static union REGS r;
    r.h.ah = 0x01;
    if (on) {
        r.h.ch = 0x06;
        r.h.cl = 0x07;
    } else {
        r.h.ch = 0x20;
        r.h.cl = 0x00;
    }
    int86(0x10, &r, &r);
}

unsigned char cgetc(void)
{
    static union REGS r;

    r.h.ah = 0x00;
    int86(0x16, &r, &r);

    if (r.h.al != 0)
        return r.h.al;

    switch (r.h.ah)
    {
    case 0x48: return ARROW_UP;
    case 0x50: return ARROW_DOWN;
    case 0x4B: return ARROW_LEFT;
    case 0x4D: return ARROW_RIGHT;
    default:   return 0;
    }
}

bool get_line(char *buf, uint8_t max_len)
{
    uint8_t c;
    uint16_t i = 0;
    unsigned char x, y;
    static union REGS r;

    r.h.ah = 0x03;
    r.h.bh = 0;
    int86(0x10, &r, &r);
    x = r.h.dl;
    y = r.h.dh;

    cursor(true);

    do
    {
        gotoxy(x + (unsigned char)i, y);
        c = cgetc();
        if (c == KEY_ESCAPE)
        {
            cursor(false);
            return false;
        }
        else if (c >= 0x20 && c < 0x7F)
        {
            unsigned int vo = (unsigned int)(y * screen_cols + x + i) * 2;
            video[vo]     = c;
            video[vo + 1] = ATTRIBUTE_NORMAL;
            buf[i] = c;
            if (i < max_len - 1)
                i++;
        }
        else if (c == KEY_BACKSPACE)
        {
            if (i > 0)
            {
                unsigned int vo;
                --i;
                vo = (unsigned int)(y * screen_cols + x + i) * 2;
                video[vo]     = ' ';
                video[vo + 1] = ATTRIBUTE_NORMAL;
            }
        }
    } while (c != ENTER);

    buf[i] = '\0';
    cursor(false);
    return true;
}

void hd_bar(unsigned char y, const char *text, unsigned char attr)
{
    char buf[81];
    int len;

    screen_fill_line(y, attr);

    len = strlen(text);
    if (len > screen_cols)
        len = screen_cols;

    screen_puts_center(y, attr, text);
}

void show_fetching_msg(bool clear)
{
    if (clear)
        screen_clear();
    screen_fill_line(0, ATTRIBUTE_NORMAL);
    screen_puts(0, 0, ATTRIBUTE_NORMAL, fetching_buf);
}
