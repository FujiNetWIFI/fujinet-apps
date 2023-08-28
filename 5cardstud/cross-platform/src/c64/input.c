#ifdef __C64__

#include <peekpoke.h>

unsigned char readJoystick() {
  return 127-PEEK(0xDC00); // joystick 2
}

#endif /* __C64__ */