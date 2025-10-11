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

int lastCoCoKey=0;

unsigned char kbhit (void) {
    return (char)(lastCoCoKey || (lastCoCoKey=inkey()) || (lastCoCoKey=inkey()));
}

char cgetc (void) {
  int key=lastCoCoKey;

  lastCoCoKey=0;

  while (!key) {
    key=inkey();
  }

  return (char)key;
}


void waitvsync() {
  uint16_t i=getTimer();
  while (i==getTimer()) {
    if (!lastCoCoKey)
      lastCoCoKey=inkey();
  }
}
#endif
