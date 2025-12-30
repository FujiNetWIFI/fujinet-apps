#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <joystick.h>
#include "vars.h"
#include "../platform-specific/graphics.h"


#define INPUT_CHARS_NUM     36		// last array index
const char input_chars[] = {" ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};

unsigned char readJoystick()
{
    return(joy_read(0));
}



#ifdef USE_PLATFORM_SPECIFIC_INPUT

void initPlatformKeyboardInput()
{

}

void getPlatformKey_helpscreen(void)
{
	unsigned char joy;

	while(1) {
		if (kbhit()) {
			cgetc();
			return;
		}

        joy = joy_read(0);
        if (joy) {
            while (joy_read(0) == joy);     // debounce joystick
        }

      	if (JOY_BTN_1(joy) || JOY_BTN_2(joy))
            return;
	}
}


// Lynx keys are 1 = Opt1, 2 = Opt2, 3 = Opt1+Opt2, R = Opt1 + Pause, F = Opt2 + Pause
int getPlatformKey_common(void)
{
	char c;

	// no key hit
	if (!kbhit()) {
		return(0);
	}

	c = cgetc();
	switch(c) {
		case '1':					// color
			return('C');
		case 'P':					// help
			return('H');
		case '2':					// refresh
			return('R');
		case '3':					// name
			return('N');
		case 'R':					// escape
			return(KEY_ESCAPE);
		case 'F':					// flip
			return('Q');
	}

	return(0);
}



int getPlatformKey_inputfield(int8_t x, int8_t y, int8_t at_max)
{
    unsigned char i, last;
    unsigned char joy;
    char s[2];


    i = 0;

	// hack so that backspace works properly
	drawText(x, y, " ");
	drawChip(x, y);

    // keys input
    while(1) {
		last = i;

        // joystick input
        joy = joy_read(0);
        if (joy) {
            while (joy_read(0) == joy);     // debounce joystick
        }

		if (!at_max) {

        if (JOY_DOWN(joy) || JOY_RIGHT(joy)) {
            if (i != 0)
                i--;
            else
                i = INPUT_CHARS_NUM;
        }

        if (JOY_UP(joy) || JOY_LEFT(joy)) {
            if (i < INPUT_CHARS_NUM)
                i++;
            else
                i = 0;
        }

		}

		// A button selects this character, or RETURN if no character selected
        if (JOY_BTN_1(joy)) {
            if (i != 0)
                return(input_chars[i]);
            else
                return(KEY_RETURN);
        }

		// B button sends backspace
        if (JOY_BTN_2(joy)) {
			drawText(x, y, " ");
            return(KEY_BACKSPACE);
        }

		// Did we change character?
		if (last != i) {
			if (i == 0) {
				drawText(x, y, " ");
				drawChip(x, y);
			}
			else {
				// Select the character from input_chars array
        		s[0] = input_chars[i];
        		s[1] = '\0';

				// draw the character on the screen
        		tgi_setcolor(lynx_bg_color);
        		tgi_bar(_char_x_scr(x), _char_y_scr(y), _char_x_scr(x)+4, _char_y_scr(y)+6);
        		tgi_setcolor(lynx_fg_color);
        		drawText(x, y, s);
			}
		}
	}
  }

#endif
