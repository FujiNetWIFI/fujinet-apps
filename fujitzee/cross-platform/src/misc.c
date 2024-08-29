/* 
 * Include CC65 style Joystick defines for Adam - There is a probably a better way to do this.
 */

#ifdef _CMOC_VERSION_
#include "coco/joystick.h"
#include "coco.h"
#include "fujinet-fuji.h"
#else
#ifdef __ADAM__
#include "adam/joystick.h"
#else
#include <joystick.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#endif /* __ADAM__ */
#endif /* _CMOC_VERSION_ */
#include "platform-specific/graphics.h"
#include "platform-specific/input.h"
#include "platform-specific/sound.h"
#include "misc.h"
#ifdef _CMOC_VERSION_
#else
#include "fujinet-fuji.h"
#endif

#ifdef _CMOC_VERSION_
#else
#include <stdbool.h>
#include <stdint.h>
#endif

unsigned char _lastJoy, _joy, _joySameCount=10;
bool _buttonReleased=true;

void pause(unsigned char frames) {
  static uint8_t i;
  for(i=0;i<frames;i++) {
      waitvsync();
  }
}

void clearCommonInput() {
  inputTrigger=inputKey=inputDirY=inputDirX=_lastJoy=_joy=_buttonReleased=0;
  while (kbhit()) 
    cgetc();
}

void readCommonInput() {
  inputTrigger=inputKey=inputDirX=inputDirY=0;

  _joy = readJoystick();

  if (_joy != _lastJoy) {
    if (_lastJoy!=99)
      _joySameCount=12;

    _lastJoy=_joy;

    if (JOY_LEFT(_joy))
      inputDirX = -1;
    else if (JOY_RIGHT(_joy)) 
      inputDirX =1;
    
    if (JOY_UP(_joy))
      inputDirY = -1;
    else if (JOY_DOWN(_joy))
      inputDirY = 1;

    // Trigger button press only if it was previously unpressed
    if (JOY_BTN_1(_joy)) {
      if (_buttonReleased) {
        inputTrigger=true;
        _buttonReleased=false;
      }
    } else {
      _buttonReleased = true;
    }
    
    return;
  } else if (_joy!=0) {
    if (!_joySameCount--) {
      _joySameCount=0;
      _lastJoy=99;
    }
  }

  inputKey=0;

  if (!kbhit())
    return;
    
  inputKey = cgetc();

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
    case KEY_SPACEBAR:
      inputTrigger=true;
      break;
  }
}

void applyPrefs() {

}

void loadPrefs() {
  read_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_PREFS, tempBuffer);
  
  if (strlen(tempBuffer)==0) {
    // Default all prefs to 1
    memset(prefs,1,2);
  } else {
    strcpy(prefs, tempBuffer);
  }

  setColorMode(prefs[PREF_COLOR]-1);
}

void savePrefs() {
  prefs[3] = 0; // Terminate prefs end as a string
  write_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_PREFS, prefs);
}


void read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint8_t* destination) {
  uint16_t read = 0;

  fuji_set_appkey_details(creator_id, app_id, DEFAULT);
  if (!fuji_read_appkey(key_id, &read, destination))
    read=0;

  // Add string terminator at end
  destination[read] = 0;
}

void write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint8_t *inputString)
{
  fuji_set_appkey_details(creator_id, app_id, DEFAULT);
  fuji_write_appkey(key_id, strlen((const char *)inputString), inputString);
}
