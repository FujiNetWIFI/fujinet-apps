/* 
 * Include CC65 style Joystick defines for Adam - There is a probably a better way to do this.
 */

#ifdef _CMOC_VERSION_
#include "coco/joystick.h"
#else
#ifdef __ADAM__
#include "adam/joystick.h"
#else
#include <joystick.h>
#include <conio.h>
#endif /* __ADAM__ */
#endif /* _CMOC_VERSION_ */
#include "platform-specific/graphics.h"

char urlBuffer[128];
unsigned char _lastJoy, _joy;

void pause(unsigned char frames) {
  unsigned char i;
  for(i=0;i<frames;i++) {
      waitvsync();
  }
}

void cycleColors() {
  static unsigned char i;
  i = cycleNextColor();
  //stub get/set app key then call graphics cycleColor
}

void clearCommonInput() {
  inputTrigger=inputKey=inputDirY=inputDirX=_lastJoy=_joy=0;
  while (kbhit()) 
    cgetc();
}

void readCommonInput() {
  inputTrigger=inputKey=inputDirX=inputDirY=0;

  _joy = readJoystick();

  if (_joy != _lastJoy) {
    _lastJoy=_joy;

    if (JOY_LEFT(_joy))
      inputDirX = -1;
    else if (JOY_RIGHT(_joy)) 
      inputDirX =1;
    
    if (JOY_UP(_joy))
      inputDirY = -1;
    else if (JOY_DOWN(_joy))
      inputDirY = 1;

    if (JOY_BTN_1(_joy))
        inputTrigger=true;

    return;
  }

  inputKey=0;

  if (!kbhit())
    return;
  inputKey = cgetc();
  switch (inputKey) {
    case 'c':
      cycleColors();
      break;
    case KEY_LEFT_ARROW:
    case KEY_LEFT_ARROW_2:
    case KEY_LEFT_ARROW_3:
      inputDirX=-1;
      break;
    case KEY_RIGHT_ARROW:
    case KEY_RIGHT_ARROW_2:
    case KEY_RIGHT_ARROW_3:
      inputDirX=1;
      break;
    case KEY_RETURN:
    case KEY_SPACE:
      inputTrigger=true;
      break;
    //default:
//      cprintf("%i",inputKey);
  }
}
