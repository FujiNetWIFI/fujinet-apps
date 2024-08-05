#ifdef _CMOC_VERSION_
/*
  Platform specific sound functions
*/

//#include <stdint.h>
//#include <stdlib.h>
//#include <apple2.h>
//#include <string.h>
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
#if 0
void initSound() {
 
}
#endif

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

void soundDealCard() {
#ifndef DISABLE_SOUND  
  static uint8_t i;

  ii=7;
  while (ii--) {
    for (i=rand()>>4; i>0; i--) ;
    CLICK;
  }
#endif  
}

void soundTick() {
#ifndef DISABLE_SOUND  
 tone(80,2,0);
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
#endif

