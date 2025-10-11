#ifdef __PLUS4__

#include <peekpoke.h>

unsigned char readJoystick() {
    return 0;
    // return 127-PEEK(0xFF00); // joystick 1
}

#endif /* __PLUS4__ */
