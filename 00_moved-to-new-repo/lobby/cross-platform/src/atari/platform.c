#ifdef __ATARI__

#include <stdint.h>
#include <atari.h>
#include <joystick.h>

void initialize() {
  OS.soundr=0; // Silent noisy SIO
}

uint8_t readJoystick() {
  return 15-OS.stick0 + (OS.strig0==0)*JOY_BTN_1_MASK;
}

void reboot() {
  OS.rtclok[2]=0;
  while (OS.rtclok[2] < 1);
  asm("JMP $E477");
}
#endif /* __ATARI__ */