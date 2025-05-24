#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "platform-specific/graphics.h"

void showInGameMenuScreen();
void progressAnim(unsigned char y);
void drawPot();
void resetStateIfNewGame();
void drawNamePurse();
void drawBets();
#ifdef __WATCOMC__
#include <stdbool.h>
void drawCards(bool finalFlip);
#else
void drawCards();
#endif /* __WATCOMC__*/
void checkIfSpectatorStatusChanged();
void checkIfPlayerCountChanged();
void checkFinalFlip();
void drawStatusTimeLeft();
void highlightActivePlayer();
void animateChipsToPotOnRoundEnd();
void drawGameStatus();
void requestPlayerMove();
void clearGameState();

#endif /*GAMELOGIC_H*/
