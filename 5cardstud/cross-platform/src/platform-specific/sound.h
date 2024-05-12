/*
  Platform specific sound functions 
*/
#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

void initSound();
bool toggleSound();
void setSound();

void soundJoinGame();
void soundMyTurn();
void soundGameDone();
void soundDealCard();
void soundTick();
void soundPlayerJoin();
void soundPlayerLeft();
void soundSelectMove();
void soundCursor();
void soundCursorInvalid();
void soundTakeChip(uint8_t counter);


#endif /* SOUND_H */