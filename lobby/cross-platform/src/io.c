#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <joystick.h>

#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "platform.h"
#include "io.h"

InputStruct input;
uint8_t _lastJoy, _joy, _joySameCount=10;
bool _buttonReleased=true;
extern char buf[];

// gotoxy + cput* saves 4 bytes over cput*xy, so why not optimize?
#define cputsxy(x,y,s) gotoxy(x,y); cputs(s);
#define cputcxy(x,y,c) gotoxy(x,y); cputc(c);


uint16_t read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, char* destination) {
  static uint16_t read;

  #ifndef _CMOC_VERSION_ // remove ifndef to enable fujinet-lib in coco
  fuji_set_appkey_details(creator_id, app_id, DEFAULT);
  if (!fuji_read_appkey(key_id, &read, (uint8_t*)destination))
    read=0;
  #endif

  // Add string terminator after the data ends in case it is being interpreted as a string
  destination[read] = 0;
  return read;
}
 
void write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id,  uint16_t count, char *data)
{
  #ifndef _CMOC_VERSION_ // remove ifndef to enable fujinet-lib in coco
    fuji_set_appkey_details(creator_id, app_id, DEFAULT);
    fuji_write_appkey(key_id, count, (uint8_t*)data);
  #endif
}

void readCommonInput() {
  input.trigger=input.key=input.dirX=input.dirY=0;

  _joy = readJoystick();

  if (_joy != _lastJoy) {
    if (_lastJoy!=99)
      _joySameCount=20;

    _lastJoy=_joy;

    if (JOY_LEFT(_joy))
      input.dirX = -1;
    else if (JOY_RIGHT(_joy)) 
      input.dirX =1;
    
    if (JOY_UP(_joy))
      input.dirY = -1;
    else if (JOY_DOWN(_joy))
      input.dirY = 1;

    // Trigger button press only if it was previously unpressed
    if (JOY_BTN_1(_joy)) {
      if (_buttonReleased) {
        input.trigger=true;
        _buttonReleased=false;
      }
    } else {
      _buttonReleased = true;
    }
    
    return;
  } else if (_joy!=0) {
    if (!_joySameCount--) {
      _joySameCount=2;
      _lastJoy=99;
    }
  }

  input.key=0;

#ifndef _CMOC_VERSION_ // until joystick is added, simple solution for cmoc for keyboard input
  if (!kbhit())
    return;
#endif
    
  input.key = cgetc();

  switch (input.key) {
    case KEY_LEFT_ARROW:
    case KEY_LEFT_ARROW_2:
    case KEY_LEFT_ARROW_3:
      input.dirX=-1;
      break;
    case KEY_RIGHT_ARROW:
    case KEY_RIGHT_ARROW_2:
    case KEY_RIGHT_ARROW_3:
      input.dirX=1;
      break;
    case KEY_UP_ARROW:
    case KEY_UP_ARROW_2:
    case KEY_UP_ARROW_3:
      input.dirY=-1;
      break;
    case KEY_DOWN_ARROW:
    case KEY_DOWN_ARROW_2:
    case KEY_DOWN_ARROW_3:
      input.dirY=1;
      break;
    case KEY_SPACEBAR:
    case KEY_RETURN:
      input.trigger=true;
      break;
  }
}

// 9085

/// @brief Custom input field with max length
void inputField(uint8_t x, uint8_t y, uint8_t max, char* buffer) {
  static uint8_t curx;
  curx = (uint8_t)strlen(buffer);
  cputsxy(x,y, buffer);
  cputc(CHAR_CURSOR);

  while (1) {
    // Process the next key press
    input.key = cgetc();

    // Debugging - See what key was pressed. Uses the extern buf[].
    //itoa(input.key, buf, 10);cputsxy(0,0, buf);

    if (input.key == KEY_RETURN && curx>1) {
      cputcxy(x+curx,y,' ');
      return;
    } else if ((input.key == KEY_BACKSPACE || input.key == KEY_LEFT_ARROW) && curx>0) {
      buffer[--curx]=0;
      cputcxy(x+1+curx,y,' ');
    } else if (
      curx < max && ((curx>0 && input.key == KEY_SPACEBAR) || (input.key>= 48 && input.key <=57) || (input.key>= 65 && input.key <=90) || (input.key>= 97 && input.key <=122))    // 0-9 A-Z a-z
    ) {

      buffer[curx]=input.key;
      buffer[++curx]=0;
    }

    cputsxy(x,y, buffer);
    cputc(CHAR_CURSOR);
  }
}