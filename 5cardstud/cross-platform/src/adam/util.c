#ifdef __ADAM__
#include <eos.h>

unsigned long currentTime;
/*
  Platform specific utilities that don't fit in any category
*/

void resetTimer()
{
  currentTime = 0;
}

int getTime()
{
  return currentTime;
}

void quit()
{
  eos_exit_to_smartwriter();
}


#endif