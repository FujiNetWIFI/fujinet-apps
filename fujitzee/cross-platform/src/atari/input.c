#ifdef __ATARI__

#include <atari.h>
#include <peekpoke.h>
#include <joystick.h>

unsigned char readJoystick() {
  return 15-OS.stick0 + (OS.strig0==0)*JOY_BTN_1_MASK; // joystick 1
}

#endif /* __ATARI__ */