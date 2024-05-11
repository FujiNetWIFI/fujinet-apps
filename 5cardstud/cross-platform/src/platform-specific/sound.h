/*
  Platform specific sound functions 
*/
#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>

void initSound();
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
bool toggleSound();

#endif /* SOUND_H */