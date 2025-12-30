/*
  Platform specific input.
*/
#ifndef INPUT_H
#define INPUT_H

// Include platform specific defines before the input include
#include "../c64/vars.h"
#include "../apple2/vars.h"
#include "../adam/vars.h"
#include "../coco/vars.h"
#include "../msdos/vars.h"
#include "../plus4/vars.h"
#include "../lynx/vars.h"

// Platform specific implementations
unsigned char readJoystick();

#ifdef USE_PLATFORM_SPECIFIC_INPUT
void initPlatformKeyboardInput();
void getPlatformKey_helpscreen(void);
int getPlatformKey_common(void);
int getPlatformKey_inputfield(int8_t x, int8_t y, int8_t at_max);

//int getPlatformKey(void);
#endif

#endif /* INPUT_H */
