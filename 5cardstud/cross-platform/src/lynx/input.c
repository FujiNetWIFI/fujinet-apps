#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <joystick.h>
#include "vars.h"

#define INPUT_CHARS_NUM     37
const char input_chars[] = {" ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};

unsigned char readJoystick()
{
    return(joy_read(0));
}



#ifdef USE_PLATFORM_SPECIFIC_INPUT

void initPlatformKeyboardInput()
{

}


int getPlatformKey(void)
{
    unsigned char i;
    unsigned char joy;
    char c, s[5];


    i = 0;
    c = 0;

    // keys input
    while(1) {
        if (kbhit()) {
            c = cgetc();
            while (kbhit());                                    // debouce key
            if (c == '2')
                return(KEY_BACKSPACE);
        }

        // joystick input
        joy = joy_read(0);
        if (joy) {
            while (joy_read(0) == joy);     // debounce joystick
        }

        if (JOY_DOWN(joy) || JOY_RIGHT(joy)) {
            if (i < INPUT_CHARS_NUM)
                i++;
            else
                i = 0;
        }

        if (JOY_UP(joy) || JOY_LEFT(joy)) {
            if (i != 0)
                i--;
            else   
                i = INPUT_CHARS_NUM-1;
        }

        c = input_chars[i];
        sprintf(s, "%d %c", i, c);
        tgi_setcolor(TGI_COLOR_GREEN);
        tgi_bar(80, 80, 130, 90);
        tgi_setcolor(TGI_COLOR_WHITE);
        tgi_outtextxy(80, 80, s);

        if (JOY_BTN_1(joy)) {
            if (i != 0)
                return(c);
            else
                return(KEY_RETURN);
        }
 
        if (JOY_BTN_2(joy)) {
            return(KEY_BACKSPACE);
        }
    }
  }

#endif
