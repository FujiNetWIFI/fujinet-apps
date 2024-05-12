#ifdef __APPLE2__
/*
  Platform specific sound functions
*/

#include <stdint.h>
#include <stdlib.h>
#include <apple2.h>
#include <string.h>
#include "../misc.h"
#include "../platform-specific/sound.h"

#define CLICK  __asm__ ("sta $c030")

uint16_t ii;
uint8_t soundOff=0;


void tone(uint16_t period, uint8_t dur, int8_t mod, uint8_t wait) {

  while (dur--) {
    for (ii=0; ii<period; ii++) ;
    CLICK;
    period += mod;
  }
  while (wait--)
    for (ii=0; ii<40; ii++) ;
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

bool toggleSound() {
  if (!soundOff)
    soundCursorInvalid();

  soundOff = !soundOff;
  soundSelectMove();

  return soundOff;
}

void setSound(bool isOff ) {
  soundOff = isOff;
}

void soundJoinGame() {
  if (soundOff)
    return;

  tone(34,40,0,115);
  tone(44,40,0,40);
  tone(28,40,0,0);
}

void soundMyTurn() {
  if (soundOff)
    return;
  tone(34,40,0,50);
  tone(34,40,0,0);
}

void soundGameDone() {
  if (soundOff)
    return;

  tone(83,20,0,20);
  tone(79,50,0,30);
  tone(65,20,0,20);
  tone(61,40,0,50);
}

void soundDealCard() {
  static uint8_t i;
  if (soundOff)
    return;
  ii=7;
  while (ii--) {
    for (i=rand()>>4; i>0; i--) ;
    CLICK;
  }
}

void soundTick() {
  if (soundOff)
    return;
 tone(80,2,0,0);
}
  
void soundPlayerJoin() {
  if (soundOff)
    return;
  for (i=255;i>=235;i-=10)
    tone(i,5,0,255);
}

void soundPlayerLeft() {
  if (soundOff)
    return;
  for (i=215;i<255;i+=10)
    tone(i,5,0,255);
}

void soundSelectMove() {
  if (soundOff)
    return;
  tone(35,40,0,20);
  tone(30,40,0,0);
}

void soundCursor() {
  if (soundOff)
    return;
  tone(45,10,0,0);
}

void soundCursorInvalid() {
  if (soundOff)
    return;
  tone(118,5,0,0);
}

void soundTakeChip(uint8_t counter) {
  if (soundOff)
    return;
  tone(118-counter*3,5,0,60);
}
#endif /* __APPLE2__ */