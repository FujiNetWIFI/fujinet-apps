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
#include "platform-specific/sound.h"

#include "stateclient.h"
#include "gamelogic.h"
#include "screens.h"

#include "fujinet-fuji.h"

// Store default server endpoint in case lobby did not set app key
//char serverEndpoint[50] = "N:https://5card.carr-designs.com/";
char serverEndpoint[50] = "N:http://127.0.0.1:8080/";
//char serverEndpoint[64] = "N:http://api.open-notify.org/iss-now.json";

char query[50] = ""; //?table=blue&player=ERICAPL2";

char playerName[12] = "";

GameState state;

// State helper vars
unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
signed char inputDirX, inputDirY;
uint16_t prevPot, rx_len, maxJifs;
bool noAnim, doAnim, finalFlip, inputTrigger;

//unsigned char playerX[8], playerY[8], moveLoc[5];
//signed char playerBetX[8], playerBetY[8], playerDir[8];

// Common local scope temp variables
unsigned char h, i, j, k, x, y, xx;
char tempBuffer[128];

char prefs[4];

char *hand, *requestedMove;

#ifdef _CMOC_VERSION_
int main(void)
#else
void main(void)
#endif /* _CMOC_VERSION_ */
{
  uint8_t skipApiCall=0, failedApiCalls=0;
  
  initGraphics(); 
  initSound();
  loadPrefs();

  showWelcomeScreen();
  showTableSelectionScreen();
  
  // Main in-game event loop
  // Gets the state from server and checks for key presses to chat/invoke menu
  while (true) {

    if (skipApiCall) {
      --skipApiCall;
    } else {
      switch (getStateFromServer()) {
        case STATE_UPDATE_ERROR:
          // Wait a bit to avoid hammering the server if getting bad responses
          failedApiCalls++;
          skipApiCall=60*failedApiCalls;
          if (skipApiCall>300)
            skipApiCall = 300;

          if (failedApiCalls>3) {
            drawStatusText("CONNECTION LOST. RECONNECTING..."); 
            
          }
          break;
        
        case STATE_UPDATE_NOCHANGE:
          // Wait a second before polling again
          skipApiCall = 600;
          break;

        case STATE_UPDATE_CHANGE:
          showGameScreen();
          requestPlayerMove();
      }
    }
    
    pause(1);

    // Chat
    handleChat();

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

/*
  // May use in future for generating player id
  AdapterConfigExtended ac;
  fuji_get_adapter_config_extended(&ac);
  
  // Remove :s
  for(i=5;i>0;--i) {
    *(uint16_t*)&ac.sMacAddress[i*2] = *(uint16_t*)&ac.sMacAddress[i*3];
  }
  ac.sMacAddress[12]=0;
*/