#ifdef __APPLE2__

#include <peekpoke.h>

unsigned char readJoystick() {
  return 127-PEEK(0xDC00); // joystick 2
}

#endif /* __APPLE2__ */