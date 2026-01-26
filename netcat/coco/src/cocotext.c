#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#include "hirestxt.h"
#include "cocotext.h"

//(byte *)*(byte *)0x00BC << 8 This is where BASIC puts it by default
#define SCREEN_BUFFER (byte*) 0xA00

/**
 * @brief Temp space for strupr(s) output, so original strings doesn't get changed.
 */
static char uppercase_tmp[321];

byte colorset = COLORSET_WHITE;
byte bgcolor = SCREEN_WHITE;
bool hirestxt_mode = false;
byte textMode = 32;
bool cursor_on = false;


void hirestxt_init(void)
{
    hirestxt_mode = true;

    // Define a `HiResTextScreenInit` object:
    struct HiResTextScreenInit init =
        {
            42,                           /* characters per row */
            writeCharAt_42cols,           /* must be consistent with previous field */
            SCREEN_BUFFER,                /* pointer to the text screen buffer */
            TRUE,                         /* redirects printf() to the 42x24 text screen */
            (word *)0x112,                /* pointer to a 60 Hz async counter (Color Basic's TIMER) */
            0,                            /* default cursor blinking rate */
            NULL,                         /* use inkey(), i.e., Color Basic's INKEY$ */
            NULL,                         /* no sound on '\a' */
        };

    width(32);                               /* PMODE graphics will only appear from 32x16 (does nothing on CoCo 1&2) */
    pmode(4, (byte *)init.textScreenBuffer); /* hires text mode */
    pcls(255);
    screen(1, colorset);
    initHiResTextScreen(&init);
}

void hirestxt_close(void)
{
    if (hirestxt_mode)
    {
        hirestxt_mode = false;
        closeHiResTextScreen();
        width(32);
        pmode(0, 0);
        screen(0, 0);
        clear_screen(255);
    }
}

void switch_colorset(void)
{
    if (hirestxt_mode)
    {
        if (colorset == COLORSET_GREEN)
        {
            colorset = COLORSET_WHITE;
        }
        else
        {
            colorset = COLORSET_GREEN;
        }

        screen(1, colorset);
    }
}

void gotoxy(byte x, byte y)
{
    if (hirestxt_mode)
    {
        moveCursor(x, y);
    }
    else
    {
        locate(x, y);
    }
}

void cursor(bool onoff)
{
	if (hirestxt_mode)
	{
        if (!cursor_on && onoff)
		{
			animateCursor();
		}
		else if (cursor_on && !onoff)
		{
			removeCursor();
		}
	}

    cursor_on = onoff;
}

void clear_screen(byte color)
{
    if (hirestxt_mode)
    {
        clrscr();
    }
    else
    {
        cls(color);
    }
}

/**
 * @brief Return uppercase string without modifying original
 */
char *screen_upper(const char *s)
{
    memset(uppercase_tmp, 0, sizeof(uppercase_tmp));
    strcpy(uppercase_tmp, s);

    return strupr(uppercase_tmp);
}

/**
 * @brief Return uppercase string without modifying original
 */
char *screen_lower(const char *s)
{
    memset(uppercase_tmp, 0, sizeof(uppercase_tmp));
    strcpy(uppercase_tmp, s);

    return strlwr(uppercase_tmp);
}

void set_text_width(byte screen_width)
{
    // Do nothing if we're not really changing the width
    if (screen_width == textMode)
    {
        return;
    }

    if (screen_width == 42)
    {
        // If we're not already in hires text mode, we need to initialize it.
        hirestxt_init();
    }
    else
    {
        if (hirestxt_mode)
        {
            hirestxt_close();
        }

        width(screen_width);
    }

    textMode = screen_width;
}


byte cgetc() 
{
    byte shift = false;
    byte k;

    while (true)
    {
        if (hirestxt_mode)
        {
            if (cursor_on)
            {  
                k = waitKeyBlinkingCursor();
            }
            else
            {
                k = inkey();
            }
        }
        else
        {
            if (cursor_on)
            {
                k = waitkey(cursor_on);
            }
            else
            {
                k = inkey();
            }
        }

		if (isKeyPressed(KEY_PROBE_SHIFT, KEY_BIT_SHIFT))
		{
			if (k == BREAK)
			{
				shift = 0x18; // Send an ESC key if SHIFT+BREAK is hit
			}
			else
			{
				shift = 0x00;
			}
		}
		else
        {
            if (k > '@' && k < '[')
            {
                shift = 0x20;
            }
        }

        return k + shift;
    }
}

void get_line(char *buf, uint8_t max_len)
{
    uint8_t c;
    uint16_t i = 0;
    uint8_t init_x = getCursorColumn();

	do
	{
		gotox((byte)i + init_x);
		cursor(true);
		c = cgetc();

		if (isprint(c))
		{
			putchar(c);
			buf[i] = c;
			if (i < max_len - 1)
				i++;
		}
		else if (c == ARROW_LEFT)
		{
			if (i)
			{
				putchar(ARROW_LEFT);
				putchar(' ');
				putchar(ARROW_LEFT);
				--i;
			}
		}
	} while (c != ENTER);
	putchar('\n');
	buf[i] = '\0';

	cursor(false);
}
