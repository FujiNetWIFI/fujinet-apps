#ifdef _CMOC_VERSION_
/*
  Platform specific utilities that don't fit in any category
*/
#include <coco.h>

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

static char lastchar;

// lastchar is only set by kbhit, we're basically handling the case
// where that key picked up by kbhit doesn't get lost.
char cgetc() {
  char c;
  if (lastchar==0)
  {
    do{
      c= inkey();
    } while(c==0);
    return c;
  }
  c= lastchar;
  lastchar= 0;
  return(c);
}

char kbhit() {
  char c;

  c= inkey();
  lastchar= c;
  return(c);
}
#endif /* _CMOC_VERSION_ */
