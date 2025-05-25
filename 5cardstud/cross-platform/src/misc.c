/*
 * Include CC65 style Joystick defines for Adam - There is a probably a better way to do this.
 */

//#ifdef __WATCOMC__
//#define USE_PLATFORM_SPECIFIC_INPUT 1
//#endif

#ifdef _CMOC_VERSION_
#include "coco/joystick.h"
#else
#ifdef __ADAM__
#include "adam/joystick.h"
#else
#ifdef __CC65__
#include <joystick.h>
#endif /* __CC65__ */
#endif /* __ADAM__ */
#include <stdio.h>
#include <string.h>
#include <conio.h>
#endif /* _CMOC_VERSION_ */
#include "platform-specific/graphics.h"
#include "platform-specific/input.h"
#include "platform-specific/sound.h"
#include "platform-specific/appkey.h"
#include "misc.h"

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#define true 1
#define false 0
#else
#include <stdbool.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
unsigned char _lastJoy, _joy;

void pause(unsigned char frames) {
  while (frames--)
    waitvsync();
}

void clearCommonInput() {
  inputTrigger=inputKey=inputDirY=inputDirX=_lastJoy=_joy=0;
#ifndef USE_PLATFORM_SPECIFIC_INPUT
  while (kbhit())
    cgetc();
#else
  //while (!getPlatformKey());
#endif
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

#ifndef USE_PLATFORM_SPECIFIC_INPUT
  if (!kbhit())
    return;

  inputKey = cgetc();
#else
  inputKey = getPlatformKey();
#endif
  if (!inputKey )
    return;

  switch (inputKey) {
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
    case KEY_UP_ARROW:
    case KEY_UP_ARROW_2:
    case KEY_UP_ARROW_3:
      inputDirY=-1;
      break;
    case KEY_DOWN_ARROW:
    case KEY_DOWN_ARROW_2:
    case KEY_DOWN_ARROW_3:
      inputDirY=1;
      break;
    case KEY_RETURN:
    case KEY_SPACE:
      inputTrigger=true;
      break;
    // case 'S':
    // case 's':
    //   prefs[PREF_SOUND] = toggleSound() ? 1 : 2;
    //   savePrefs();
    //   break;
  }
}

void applyPrefs() {

}

void loadPrefs() {
  read_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_PREFS, tempBuffer);

  if (strlen(tempBuffer)==0) {
    // Default all prefs to 1
    memset(prefs,1,3);
  } else {
    strcpy(prefs, tempBuffer);
  }

  //setSound(prefs[PREF_SOUND]==1);
  setColorMode(prefs[PREF_COLOR]-1);
}

void savePrefs() {
  prefs[3] = 0; // Terminate prefs end as a string
  write_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_PREFS, prefs);
}
