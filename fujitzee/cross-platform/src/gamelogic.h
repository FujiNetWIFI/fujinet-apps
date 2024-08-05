#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "platform-specific/graphics.h"

void processStateChange();
void processInput();
void handleAnimation();


void clearGameState();
void forceRender();

void centerText(unsigned char y, char * text);
void centerTextAlt(unsigned char y, char * text);
void centerTextWide(unsigned char y, char * text);
void centerStatusText(char * text);

bool inputFieldCycle(uint8_t x, uint8_t y, uint8_t max, uint8_t* buffer);


void renderBoardNamesMessages();

void waitOnPlayerMove();


void progressAnim(unsigned char y);


#endif /*GAMELOGIC_H*/
