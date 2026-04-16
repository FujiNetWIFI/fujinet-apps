#include <dos.h>
#include <string.h>
#include "bar.h"
#include "screen.h"

static int bar_y = 3, bar_c = 1, bar_m = 1, bar_i = 0, bar_oldi = 0;

static unsigned char saved_current[80];
static unsigned char saved_next[80];
static int saved_current_row = -1;

static void save_row_attrs(int y, unsigned char *buf)
{
    int o = (y * screen_cols + bar_c) * 2 + 1;
    int w = screen_cols - 2 * bar_c;
    int i;
    if (w > 80) w = 80;
    for (i = 0; i < w; i++) { buf[i] = (unsigned char)video[o]; o += 2; }
}

static void restore_row_attrs(int y, unsigned char *buf)
{
    int o = (y * screen_cols + bar_c) * 2 + 1;
    int w = screen_cols - 2 * bar_c;
    int i;
    if (w > 80) w = 80;
    for (i = 0; i < w; i++) { video[o] = buf[i]; o += 2; }
}

void bar_set(unsigned char y, unsigned char c, unsigned char m, unsigned char i)
{
    bar_oldi = bar_i;
    bar_y = y;
    bar_c = c;
    bar_m = (m == 0 ? 0 : m - 1);
    bar_i = i;
    bar_update();
}

void bar_clear(bool old)
{
    if (old)
    {
        bar_draw(bar_y + bar_oldi, true);
    }
    else
    {
        int row = bar_y + bar_i;
        if (saved_current_row == row)
            restore_row_attrs(row, saved_current);
        else
            bar_draw(row, true);
        saved_current_row = -1;
    }
}

void bar_draw(int y, bool clear)
{
    int o = (y * screen_cols + bar_c) * 2 + 1;
    int w = screen_cols - 2 * bar_c;
    int i;

    for (i = 0; i < w; i++)
    {
        video[o] = clear ? ATTRIBUTE_NORMAL : ATTRIBUTE_SELECTED;
        o += 2;
    }
}

uint8_t bar_get(void)
{
    return bar_i;
}

void bar_up(void)
{
    bar_oldi = bar_i;
    if (bar_i > 0)
    {
        bar_i--;
        bar_update();
    }
}

void bar_down(void)
{
    bar_oldi = bar_i;
    if (bar_i < bar_m)
    {
        bar_i++;
        bar_update();
    }
}

void bar_update(void)
{
    int new_row = bar_y + bar_i;
    int old_row = bar_y + bar_oldi;
    int i, w;

    save_row_attrs(new_row, saved_next);

    if (saved_current_row == old_row)
        restore_row_attrs(old_row, saved_current);
    else
        bar_draw(old_row, true);

    w = screen_cols - 2 * bar_c;
    if (w > 80) w = 80;
    for (i = 0; i < w; i++) saved_current[i] = saved_next[i];
    saved_current_row = new_row;

    bar_draw(new_row, false);
}
