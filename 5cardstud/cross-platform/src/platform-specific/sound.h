/*
  Platform specific sound functions 
*/
#ifndef SOUND_H
#define SOUND_H

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdbool.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */

void initSound();


/* Will bring back sound toggle if there is a platform with no controllable sound volume
bool toggleSound();
void setSound();
*/

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
void soundTakeChip(uint16_t counter);


#endif /* SOUND_H */
