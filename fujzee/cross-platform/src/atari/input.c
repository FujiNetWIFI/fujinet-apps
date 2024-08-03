#ifdef __ATARI__

#include <peekpoke.h>

unsigned char readJoystick() {
  return 15-PEEK(0x278); // joystick 1
}

#endif /* __ATARI__ */