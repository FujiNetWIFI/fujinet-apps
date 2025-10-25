#ifdef _CMOC_VERSION_

/*
  Platform specific utilities that don't fit in any category
*/
#include <cmoc.h>
#include <coco.h>
#include "hires.h"

void resetTimer()
{
  setTimer(0);
}

int getTime()
{
  return getTimer();
}

void quit()
{
  pmode(0, 0x400);
  pcls(0x60);
  screen(0,0);
  memset(0x200,0,0x200);
  memset(0x600,0,0x1200);
  exit(0);
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
