#ifdef __C64__

#include<peekpoke.h>
#include<stdlib.h>

void resetGraphics();

void resetTimer() {
  POKE(162,0);
  POKE(161,0);
}

int getTime() {
  return (PEEK(161)*256)+PEEK(162);
}

void quit() { 
  resetGraphics();
  exit(0);
}

#endif /* __C64__ */