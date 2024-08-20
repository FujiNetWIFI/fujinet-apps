#ifdef _CMOC_VERSION_
/*
  Platform specific sound functions
*/

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"

#include "../misc.h"
#include "../platform-specific/sound.h"

//#define CLICK  __asm__ ("sta $c030")
#define CLICK 

uint16_t ii;

void tone(uint16_t period, uint8_t dur, uint8_t wait) {
#ifndef DISABLE_SOUND  
  while (dur--) {
    for (ii=0; ii<period; ii++) ;
    CLICK;
  }

  while (wait--)
    for (ii=0; ii<40; ii++) ;
#endif  
}

// Keeping this here in case I need it
// void toneFinder() {
//   clearCommonInput();
//   while (inputKey != KEY_RETURN || i<2) {
//     while (!kbhit());
//     inputKey = cgetc();
//     if (inputKey == KEY_DOWN_ARROW)
//       i-=1;
//     if (inputKey == KEY_UP_ARROW)
//       i+=1;
//       cprintf("%i ",i);
//     tone(i,50,0,0);
//   }
// }
void initSound() {
 
}

void soundJoinGame() {
#ifndef DISABLE_SOUND  
  tone(34,40,115);
  tone(44,40,40);
  tone(28,40,0);
#endif  
}

void soundMyTurn() {
#ifndef DISABLE_SOUND  
  tone(34,40,50);
  tone(34,40,0);
#endif  
}

void soundGameDone() {
#ifndef DISABLE_SOUND  
  tone(83,20,20);
  tone(79,50,30);
  tone(65,20,20);
  tone(61,40,50);
#endif  
}

void soundRollDice() {
#ifndef DISABLE_SOUND
  _sound(0, 150+ (rand() % 20)*5,8,7);
#endif
}

void soundTick() {
#ifndef DISABLE_SOUND  
 tone(80,2,0);
#endif  
}

void soundStop() {
#ifndef DISABLE_SOUND
  _sound(0,0,0,0);
#endif
}

void soundPlayerJoin() {
#ifndef DISABLE_SOUND  
  for (i=255;i>=235;i-=10)
    tone(i,5,255);
#endif  
}

void soundPlayerLeft() {
#ifndef DISABLE_SOUND  
  for (i=215;i<255;i+=10)
    tone(i,5,255);
#endif  
}

void soundSelectMove() {
#ifndef DISABLE_SOUND  
  tone(35,40,20);
  tone(30,40,0);
#endif  
}

void soundCursor() {
#ifndef DISABLE_SOUND  
  tone(45,10,0);
#endif  
}

void soundCursorScore() {
#ifndef DISABLE_SOUND
  _sound(0,90,10,5);
  pause(1);
  soundStop();
#endif
}

void soundKeep() {
#ifndef DISABLE_SOUND
  j=0;
  for(i=200;i>150;i-=10) {
    _sound(0,i,10,2+j++);
    waitvsync();
  }
  soundStop();
#endif
}

void soundRelease() {
#ifndef DISABLE_SOUND
  for(i=6;i<255;i--) {
    _sound(0,200,10,i);
    waitvsync();
  }
#endif
}

void soundCursorInvalid() {
#ifndef DISABLE_SOUND  
  tone(118,5,0);
#endif  
}

void soundTakeChip(uint16_t counter) {
#ifndef DISABLE_SOUND  
  tone(118-counter*3,5,60);
#endif  
}

void disableKeySounds() {
#ifndef DISABLE_SOUND
  OS.noclik = 255;
#endif
}

void enableKeySounds() {
#ifndef DISABLE_SOUND
  OS.noclik = 0;
#endif
}

void soundScore() {
#ifndef DISABLE_SOUND 
  j=0;
  for(i=80;i>50;i-=10) {
    _sound(0,i,10,2+j++);
    waitvsync();
  }
  soundStop();
#endif
}

#endif

