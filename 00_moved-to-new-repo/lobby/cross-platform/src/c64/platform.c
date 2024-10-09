#ifdef __C64__

#include <stdint.h>
#include <peekpoke.h>

void initialize() {
  // Nothing for C64
}

uint8_t readJoystick() {
  return 127-PEEK(0xDC00); // joystick 2
}

void reboot() {
  // TBD

  // Soft reboot
  //__asm__("JMP $FCE2");
  
  // instead of boot, load the program?
  //cbm_load(...);
}
#endif /* __C64__ */