#ifdef _CMOC_VERSION_

/*
  Platform specific utilities that don't fit in any category
*/
#include <cmoc.h>
#include <coco.h>
#include "hires.h"

void resetTimer()
{
}

int getTime()
{
  return 0;
}

void quit()
{
}

char lastKey=0;

unsigned char kbhit (void) {
    return lastKey=(char)inkey();
}

char cgetc (void) {
    char key=lastKey;

    lastKey=0;

    while (!key) {
        key=(char)inkey();
    }

    return key;
}

#endif
