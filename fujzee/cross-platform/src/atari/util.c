#ifdef __ATARI__

#include<peekpoke.h>
#include<stdlib.h>
#include<stdint.h>

void resetGraphics();
uint16_t jiffieTimer;

void resetTimer() {
  jiffieTimer=0;
}

int getTime() {
  jiffieTimer+=3;
  return jiffieTimer;
}

void quit() {
  // Possible to revert screen to boot normals
  resetGraphics();
  exit(0);
}

#endif /* __ATARI__ */