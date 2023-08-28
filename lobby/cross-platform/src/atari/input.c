#ifdef __ATARI__

/**
 * Cross platform Input functions
 */

#include <atari.h>

unsigned char readJoystick() {
  // Returns standardized joystick response, triggering fire on OPTION
  // and stick down on SELECT
  return JOY_BTN_1_MASK*(!OS.strig0||!(GTIA_READ.consol&4))+
        +JOY_DOWN_MASK*!(GTIA_READ.consol&2)+
        15-OS.stick0;
}

#endif /* __ATARI__ */