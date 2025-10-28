#ifdef _CMOC_VERSION_
#include "coco/coco_bool.h"
#include <cmoc.h>
#include <coco.h>
unsigned char kbhit(void);
char cgetc(void);
#else
#include <stdlib.h>
#ifdef __WATCOMC__
//#define USE_PLATFORM_SPECIFIC_INPUT 1
#else
#include <conio.h>
#endif /* __WATCOMC__ */
#include <string.h>
#endif /* _CMOC_VERSION_ */
#include "stateclient.h"
#include "screens.h"
#include "misc.h"
#include "gamelogic.h"

#include "platform-specific/graphics.h"
#include "platform-specific/input.h"
#include "platform-specific/appkey.h"
#include "platform-specific/util.h"
#include "platform-specific/sound.h"

#define PLAYER_NAME_MAX 8

#ifdef HIRES_Y
unsigned char y_bump = 1;
#define BUMP_LINE y_bump+=4;
#define CLEAR_BUMP y_bump=1;
#else
#define BUMP_LINE y++;
#define CLEAR_BUMP
#endif

#define TABLE_LEFT (WIDTH/2-13)

unsigned char redrawGameScreen = 0;

/// @brief Convenience function to draw text centered at row Y
void centerText(unsigned char y, const char * text) {
#ifdef HIRES_Y
  drawTextAt((unsigned char)(WIDTH/2-strlen(text)/2), y*8+y_bump, text);
#else
  drawText((unsigned char)(WIDTH/2-strlen(text)/2), y, text);
#endif
}

/// @brief Convenience function to draw status text centered
void centerStatusText(const char * text) {
  clearStatusBar();
  drawStatusTextAt((unsigned char)((WIDTH-strlen(text))>>1),text);
}

/// @brief Convenience function to reset screen and draw border
void resetScreenWithBorder() {
  resetScreen();
  drawBorder();
}

/// @brief Shows information about the game
void showHelpScreen() {
  static unsigned char y;
  enableDoubleBuffer();
  centerStatusText("LOADING INSTRUCTIONS..");
  drawBuffer();

  // This COULD be retrieved from the server, especially if
  // this client were game agnostic.
  resetScreenWithBorder();
  centerText(3,"HOW TO PLAY 5 CARD STUD");
  y=4;
  
  BUMP_LINE
                  //12345678901234567890123456789012
  y++;centerText(y, "PLAYERS ARE DEALT 5 CARDS OVER");
  y++;centerText(y, "FOUR ROUNDS.  IN EVERY ROUND, ");
  y++;centerText(y, "PLAYERS BET, CALL, AND RAISE. ");
  y+=2;
  
  centerText(y, "MOVES");
  
  BUMP_LINE        //12345678901234567890123456789012
  y++;centerText(y, "FOLD  - QUIT THE HAND        ");BUMP_LINE
  y++;centerText(y, "CHECK - FREE PASS            ");BUMP_LINE
  y++;centerText(y, "BET / - INCREASE BET. OTHERS ");
  y++;centerText(y, "RAISE   MUST CALL TO STAY IN ");BUMP_LINE
  y++;centerText(y, "CALL  - MATCH THE CURRENT BET");

  CLEAR_BUMP

  centerStatusText("PRESS ANY KEY TO CONTINUE");

  drawBuffer();

  clearCommonInput();
#ifdef USE_PLATFORM_SPECIFIC_INPUT
  getPlatformKey();
#else
  cgetc();
#endif
  resetScreen();
  drawBuffer();
  redrawGameScreen=1;
}

/// @brief Action called in Welcome Screen to check if a server name is stored in an app key
void welcomeActionVerifyServerDetails() {
  // Read server endpoint into server and query
  read_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, tempBuffer);

  i=(unsigned char)strlen(tempBuffer);
  if (i) {

    // Split "server?query" into separate variables
    while(--i) {
      if (tempBuffer[i]=='?') {
        strcpy(query, tempBuffer+i);
        tempBuffer[i]=0;
        strcpy(serverEndpoint, tempBuffer);
        break;
      }
    }
  }
}




bool inputFieldCycle(uint8_t x, uint8_t y, uint8_t max, char* buffer) {
  static uint8_t done, curx, lastY;

  // Initialize first call to input box
  //i=strlen(buffer);
  if (done == 1 || lastY != y) {
    done=0;
    lastY=y;
    curx = (unsigned char)strlen((char *)buffer);
    drawText(x,y, (const char *)buffer);
    drawChip(x+curx,y);
  }
   // curx=i;
   // done=0;
 // }


  // Process any waiting keystrokes
  #ifdef USE_PLATFORM_SPECIFIC_INPUT
  inputKey = getPlatformKey();
  if (inputKey) {
    done=0;
    disableDoubleBuffer();
  #else
  if (kbhit()) {
    done=0;
    disableDoubleBuffer();
    inputKey = cgetc();
  #endif

    if ((inputKey == KEY_RETURN ) && curx>1) {
      done=1;
      // remove cursor
      drawText(x+1+i,y," ");
    } else if (inputKey == KEY_BACKSPACE && curx>0) {
      buffer[--curx]=0;
      drawText(x+1+curx,y," ");
    } else if (
      curx < max && ((curx>0 && inputKey == KEY_SPACE) || (inputKey>= 97 && inputKey <=122) ||(inputKey>= 48 && inputKey <=57) || (inputKey>= 65 && inputKey <=90))    // 0-9 A-Z
    ) {
        buffer[curx]=(unsigned char)inputKey;
      buffer[++curx]=0;
    }

    drawText(x,y, (const char *)buffer);
    drawChip(x+curx,y);

    return done==1;
  }

  return false;

}

void showPlayerNameScreen() {
  enableDoubleBuffer();
  resetScreenWithBorder();
  clearStatusBar();
  drawLogo();

  drawBuffer();
  disableDoubleBuffer();

  centerText(13, "ENTER YOUR NAME");
  drawBox(WIDTH/2-PLAYER_NAME_MAX/2-1,16,PLAYER_NAME_MAX+1,1);
  drawText(WIDTH/2-PLAYER_NAME_MAX/2,17, playerName);

  i=(unsigned char)strlen(playerName);

  clearCommonInput();
  while (!inputFieldCycle(WIDTH/2-PLAYER_NAME_MAX/2, 17, PLAYER_NAME_MAX, (uint8_t *)playerName)) ;

  enableDoubleBuffer();
  for (y=13;y<19;++y)
    centerText(y, "                 ");

  drawBuffer();

  write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, playerName);
}

/// @brief Action called in Welcome Screen to verify player has a name
void welcomeActionVerifyPlayerName() {
  // Read player's name from app key
  read_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, tempBuffer);

  tempBuffer[12]=0;
  if (tempBuffer[0] != 0)
    strcpy(playerName,tempBuffer);

  // Capture username if player didn't come in from the lobby
  if (strlen(playerName) == 0)
    showPlayerNameScreen();
}

/// @brief Shows the Welcome Screen with Logo. Asks player's name
void showWelcomScreen() {
  resetScreenWithBorder();
  drawLogo();

  welcomeActionVerifyPlayerName();
  welcomeActionVerifyServerDetails();

  strcpy(tempBuffer, "WELCOME ");
  strcat(tempBuffer, playerName);
  centerText(13,tempBuffer);
  drawBuffer();
  pause(45);


  // If first run, show the help screen
  if (prefs[PREF_HELP]!=2) {
    prefs[PREF_HELP]=2;
    savePrefs();
    showHelpScreen();

  }
  pause(30);
}

/// @brief Action in Table Selection Screen, joins the selected server
void tableActionJoinServer() {
  // Reset the game state
  clearGameState();

  strcat(query, "&player=");
  strcat(query, playerName);

  // Replace space with + for pshowWelcomScreenlayer name
  i=(unsigned char)strlen(query);
  while(--i)
    if (query[i]==' ')
      query[i]='+';

  redrawGameScreen=1;
}

/// @brief Shows a screen to select a table to join
void showTableSelectionScreen() {
  static uint8_t shownChip;
  static unsigned char tableIndex=0;
  static uint8_t skipApiCall;
  static Table* table;
  skipApiCall=0;
  // An empty query means a table needs to be selected
  while (strlen(query)==0) {

    // SkipApiCall will be true if toggling color, to redraw

    if (!skipApiCall) {
      // Show the status immediately before retrival
      centerStatusText("REFRESHING TABLE LIST..");
      drawBuffer();
    }

    resetScreenWithBorder();
    
    centerText(3, "CHOOSE A TABLE TO JOIN");
    drawText(TABLE_LEFT,6, "TABLE");
    drawText(TABLE_LEFT+19,6, "PLAYERS");
    drawLine(TABLE_LEFT,7,26);
    
    drawBuffer();
    waitvsync();

    if (!skipApiCall){
      apiCall("tables");
    }

    skipApiCall=0;

    if (clientState.tables.count>0) {
      for(i=0;i<clientState.tables.count;++i) {
        table = &clientState.tables.table[i];
        drawText(TABLE_LEFT,8+i*2, table->name);
        drawText((unsigned char)(TABLE_LEFT+26-strlen(table->players)), 8+i*2, table->players);
        
        if (table->players[0]>'0') {
          drawText((unsigned char)(TABLE_LEFT+24-strlen(table->players)), 8+i*2, "*");  
        }
      }
    } else {
      centerText(12, "NO TABLES ARE AVAILABLE");
    }
    
    //drawStatusText(" R+EFRESH  H+ELP  C+OLOR  S+OUND  Q+UIT");
#if WIDTH>=40 
    drawStatusText("R-EFRESH   H-ELP  C-OLOR   N-AME   Q-UIT");
#else               //12345678901234567890123456789012
    drawStatusText("R-EFRESH   H-ELP   N-AME   Q-UIT");
#endif
    drawBuffer();
    disableDoubleBuffer();
    shownChip=0;

    clearCommonInput();
    while (!inputTrigger || !clientState.tables.count) {
      readCommonInput();
      //if (kbhit())
        //inputKey = cgetc();
      if (inputKey == 'h' || inputKey == 'H') {
        showHelpScreen();
        break;
      } else if (inputKey == 'r' || inputKey =='R') {
        break;
      } else if (inputKey == 'c' || inputKey =='C') {
        prefs[PREF_COLOR] = cycleNextColor()+1;
        savePrefs();
        enableDoubleBuffer();
        skipApiCall=1;
        break;
        } else if (inputKey == 'n' || inputKey =='N') {
        showPlayerNameScreen();
        break;
      } else if (inputKey == 'q' || inputKey =='Q') {
        quit();
      } /*else if (inputKey != 0) {
        itoa(inputKey, tempBuffer, 10);
        drawStatusText(tempBuffer);
      } */

      if (clientState.tables.count>0 && (!shownChip || inputDirY)) {

        drawText(TABLE_LEFT-1,8+tableIndex*2," ");
        tableIndex+=inputDirY;
        if (tableIndex==255)
          tableIndex=clientState.tables.count-1;
        else if (tableIndex>=clientState.tables.count)
          tableIndex=0;

        drawChip(TABLE_LEFT-1,8+tableIndex*2);

        soundCursor();
        shownChip=1;
      }
    }

    enableDoubleBuffer();

    if (inputTrigger) {
      soundSelectMove();

      // Clear screen and write server name
      resetScreenWithBorder();
      clearStatusBar();
      centerText(15, clientState.tables.table[tableIndex].name);

      strcpy(query, "?table=");
      strcat(query, clientState.tables.table[tableIndex].table);
      strcpy(tempBuffer, serverEndpoint);
      strcat(tempBuffer, query);

      //  Update server app key in case of reboot
      write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, tempBuffer);

    }
  
  }

  centerText(17, "CONNECTING TO SERVER");
  drawBuffer();

  progressAnim(19);

  tableActionJoinServer();
}

/// @brief Shows main game play screen (table and cards)
void showGameScreen() {
  /*

typedef struct {
  char name   [9];
  uint8_t     status;
  uint16_t    bet;
  char move   [8];
  uint16_t    purse;
  char hand   [11];
} Player;

#ifndef SINGLE_BUFFER
  redrawGameScreen=1;
#endif

typedef struct {
  char lastResult[81];
  uint8_t round;
  uint16_t pot;
  int8_t activePlayer;
  uint8_t moveTime;
  uint8_t viewing;
  uint8_t validMoveCount;
  ValidMove validMoves[5];
  uint8_t playerCount;
  Player players[8];
} Game;
 */
  // printf("\r\nround=%u, pot=%u, ap=%s, m=%u,v=%u",state.round, state.pot, state.activePlayer, state.moveTime, state.viewing);
  // printf("\r\nvalidMoves=%u, playerCount=%u\r\n",state.validMoveCount, state.playerCount);

  // for(i=0;i<state.validMoveCount;++i) {
  //   printf(" %s=%s,",state.validMoves[i].move, state.validMoves[i].name);
  // }
  // printf("\r\n");
  // for(i=0;i<state.playerCount;++i) {
  //   printf("\r\n%s=%u, bet=%u |%s| p=%u, H=%s",state.players[i].name, state.players[i].status, state.players[i].bet, state.players[i].move, state.players[i].purse, state.players[i].hand);
  // }

  // printf("\r\n");
  // drawBuffer();
  // cgetc();


  checkIfSpectatorStatusChanged();
  checkIfPlayerCountChanged();

  // Animate chips to pot before drawing the new state
  animateChipsToPotOnRoundEnd();

  checkFinalFlip();

  if (redrawGameScreen) {
    resetScreen();
  }

  resetStateIfNewGame();

  drawPot();

  if (state.playerCount>1) {
    drawNamePurse();
    drawBets();
    if (state.round < 5) {
      drawCards(false);
    }
  }

  drawGameStatus();
  drawBuffer();
  highlightActivePlayer();

  redrawGameScreen=0;
  prevRound = state.round;
}

/// @brief shows in-game menu
void showInGameMenuScreen() {

  hideLine(0,0,0);
  i=1;
  while (i) {
    enableDoubleBuffer();
    clearStatusBar();
    resetScreenWithBorder();
    drawBuffer();

    x = WIDTH/2-9;
    y = HEIGHT/2-4;

    drawBox(x-3,y-3,22,11);
    drawText(x,y,    "  Q: QUIT TABLE");
    drawText(x,y+=2, "  H: HOW TO PLAY");
    drawText(x,y+=2, "  C: COLOR TOGGLE");
    //drawText(x,y+=2, "  S: SOUND TOGGLE");
    drawText(x,y+=2, "ESC: KEEP PLAYING");
    drawBuffer();

    clearCommonInput();

    i=1;
    while (i==1) {
      readCommonInput();
      switch (inputKey) {
        case 'c':
        case 'C':
            prefs[PREF_COLOR] = cycleNextColor()+1;
            savePrefs();
            i=2;
            break;
        case 'h':
        case 'H':
          showHelpScreen();
        case KEY_ESCAPE:
        case KEY_ESCAPE_ALT:
          i=0;
          break;
        case 'q':
        case 'Q':
          resetScreenWithBorder();
          centerText(10, "PLEASE WAIT");
          drawBuffer();

          // Inform server player is leaving
          apiCall("leave");
          progressAnim(12);
          
          //  Clear server app key in case of reboot
          write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, "");

          // Clear query so a new table will be selected
          clearCommonInput();
          strcpy(query,"");
          showTableSelectionScreen();
          return;
      }
    }
  }

  // Show game screen again before returning
  redrawGameScreen=1;
  resetScreen();
  drawBuffer();
  showGameScreen();
  clearCommonInput();
}
