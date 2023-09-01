/**
 * @brief   5-card-stud
 * @author  Eric Carr, Thomas Cherryhomes, (insert names here)
 * @license gpl v. 3
 * @verbose main
 */ 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// FujiNet AppKey settings. These should not be changed
#define AK_LOBBY_CREATOR_ID 1     // FUJINET Lobby
#define AK_LOBBY_APP_ID 1         // Lobby Enabled Game
#define AK_LOBBY_KEY_USERNAME 0   // Lobby Username key
#define AK_LOBBY_KEY_SERVER 1     // 5 Card Stud Client registered as Lobby appkey 1

// 5 Card Stud client
#define AK_CREATOR_ID 0xE41C      // Eric Carr//s creator id
#define AK_APP_ID 1               // 5 Card/Poker App ID
#define AK_KEY_SHOWHELP 0         // Shown help
#define AK_KEY_COLORTHEME 1       // Color theme

// Store default server endpoint in case lobby did not set app key
char serverEndpoint[64] = "https://5card.carr-designs.com/";
char query[64] = ""; //?table=blue&player=C64ERIC";
char playerName[64] = "";

typedef struct {
  char * table;
  char * name;
  char players[8];
} Table;

typedef struct {
  char * name;
  unsigned char status;
  int bet;
  char * move;
  int purse;      
  char * hand;
} Player;

typedef struct {
  char * move;
  char * name; 
} ValidMove;

typedef struct {
  char * lastResult;
  unsigned char round;
  int pot;
  signed char activePlayer;
  unsigned char moveTime;
  unsigned char viewing;
  ValidMove validMoves[5];
  Player players[8];
  Table tables[10];

} GameState;

GameState state;



// State helper vars
unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
signed char inputDirX, inputDirY;
int prevPot, rx_len, maxJifs;

unsigned char playerX[8], playerY[8], moveLoc[5];
signed char playerBetX[8], playerBetY[8], playerDir[8];
bool noAnim, doAnim, finalFlip, inputTrigger;

// Common local scope temp variables
unsigned char h, i, j, k, x, y, xx;
char tempBuffer[255];
char *hand, *requestedMove;

#include "platform-specific/graphics.h"
#include "platform-specific/util.h"
#include "platform-specific/input.h"
#include "misc.h"
#include "platform-specific/network.h"
#include "platform-specific/appkey.h"

#include "stateclient.h"
#include "gamelogic.h"
#include "screens.h"

void main(void) {
  initGraphics();
  
  showWelcomScreen();  
  showTableSelectionScreen();
  
  // Main in-game loop
  while (true) {

    // Get latest state and draw on screen, then prompt player for move if their turn
    if (getStateFromServer()) {
      showGameScreen();
      requestPlayerMove();
    } else {
      pause(10);
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
}
