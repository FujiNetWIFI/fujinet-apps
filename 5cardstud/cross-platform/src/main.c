/**
 * @brief   5-card-stud
 * @author  Eric Carr, Thomas Cherryhomes, (insert names here)
 * @license gpl v. 3
 * @verbose main
 */

#ifdef _CMOC_VERSION_
#include <cmoc.h>
typedef unsigned char bool;
#define false 0
#define true 1
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#include "platform-specific/graphics.h"
#include "platform-specific/util.h"
#include "platform-specific/input.h"
#include "misc.h"
#include "platform-specific/network.h"
#include "platform-specific/appkey.h"
#include "platform-specific/sound.h"

#include "stateclient.h"
#include "gamelogic.h"
#include "screens.h"

// Store default server endpoint in case lobby did not set app key
//char serverEndpoint[50] = "N:https://5card.carr-designs.com/";
char serverEndpoint[50] = "N:http://192.168.2.41:8080/";
//char serverEndpoint[64] = "N:http://api.open-notify.org/iss-now.json";

char query[50] = ""; //?table=blue&player=ERICAPL2";
char playerName[12] = "";

GameState state;

// State helper vars
unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
signed char inputDirX, inputDirY;
int prevPot, rx_len, maxJifs;
bool noAnim, doAnim, finalFlip, inputTrigger;

unsigned char playerX[8], playerY[8], moveLoc[5];
signed char playerBetX[8], playerBetY[8], playerDir[8];

// Common local scope temp variables
unsigned char h, i, j, k, x, y, xx;
char tempBuffer[128];

char prefs[4];

char *hand, *requestedMove;

//#include <stdio.h>

#ifdef _CMOC_VERSION_
int main(void)
#else
void main(void)
#endif /* _CMOC_VERSION_ */
{
  loadPrefs();
  
  initGraphics(); 
  initSound();

  showWelcomScreen();
  showTableSelectionScreen();
  
  // Main in-game loop
  while (true) {

    // Get latest state and draw on screen, then prompt player for move if their turn
    if (getStateFromServer()) {
      showGameScreen();
      requestPlayerMove();
    } else {
       // Wait a bit to avoid hammering the server if getting bad responses
       pause(30);
    }
    
    // Handle other key presses
    readCommonInput();
    
    switch(inputKey) {
      case KEY_ESCAPE: // Esc
      case KEY_ESCAPE_ALT: // Esc Alt
        showInGameMenuScreen();  
        break;
    }
   
    
  }

#ifdef _CMOC_VERSION_
  return 0;
#endif /* CMOC_VERSION_  */
}
