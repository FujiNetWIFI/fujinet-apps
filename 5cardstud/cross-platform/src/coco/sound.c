#ifdef _CMOC_VERSION_
/*
  Platform specific sound functions
*/
#include <coco.h>
#include "../misc.h"
#include "../platform-specific/sound.h"

uint16_t ii;


void tone(uint8_t period, uint8_t dur, uint8_t wait) {
  //if (!prefs.disableSound) {
    sound(period, dur);
  //}
  
  while (wait--)
    for (ii=0; ii<60; ii++) ;
}

// // Keeping this here in case I need it
// void toneFinder() {
//   clearCommonInput();
//   while (input.key != KEY_RETURN || i<2) {
//     while (!kbhit());
//     input.key = cgetc();
//     switch (input.key) {
//       case KEY_DOWN_ARROW: i--; break;
//       case KEY_UP_ARROW: i++; break;
//       case KEY_RIGHT_ARROW: i+=20; break;
//       case KEY_LEFT_ARROW: i-=20; break;
//       case KEY_ESCAPE: return;
//     }
//     printf("%d ",i);
//     tone(i,1,0);
//   }
// }

void initSound() {}


void soundJoinGame() {

}

void soundMyTurn() {
  tone(100,1,40);
  tone(100,2,0);
}

void soundGameDone() {

}

void soundDealCard() {
  static uint8_t i;
  for(i=0;i<7;i++)
    tone((i*13+8)%100,0,0);
}

void soundTick() {
  tone(0,0,0);
}

void soundPlayerJoin() {

}

void soundPlayerLeft() {

}

void soundSelectMove() {
  tone(60,1,0);
  tone(100,1,0);
}

void soundCursor() {
  tone(80,1,0);
}

void soundCursorInvalid() {
  tone(0,1,0);
}

void soundTakeChip(uint16_t counter) {
  tone(counter*3,1,4);
}
#endif
