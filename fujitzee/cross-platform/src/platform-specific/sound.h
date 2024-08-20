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
#endif

void initSound();

void disableKeySounds();
void enableKeySounds();

/* Will bring back sound toggle if there is a platform with no controllable sound volume
bool toggleSound();
void setSound();
*/

void soundStop();
void soundJoinGame();
void soundMyTurn();
void soundGameDone();
void soundRollDice();
void soundTick();
void soundPlayerJoin();
void soundPlayerLeft();

void soundCursor();
void soundCursorScore();
void soundKeep();
void soundRelease();
void soundScore();



#endif /* SOUND_H */
