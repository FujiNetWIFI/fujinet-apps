#ifdef _CMOC_VERSION_
#include "coco/coco_bool.h"
#include <coco.h>
#else
#include <stdlib.h>
#include <conio.h>
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

#ifdef _CMOC_VERSION_
// lastchar is only set by kbhit, we're basically handling the case
// where that key picked up by kbhit doesn't get lost.
char cgetc() {
  return inkey();
}

char kbhit() {
  return inkey();
}
#endif

/// @brief Convenience function to draw text centered at row Y
void centerText(unsigned char y, char * text) {
  drawText((unsigned char)(WIDTH/2-strlen(text)/2), y, text);
}

/// @brief Convenience function to draw status text centered
void centerStatusText(char * text) {
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
  //                  // __________________________________
  y++;drawText(3,y, "PLAYERS ARE DEALT 5 CARDS OVER THE");
  y++;drawText(3,y, "COURSE OF 4 ROUNDS. IN EACH ROUND");
  y++;drawText(3,y, "PLAYERS BET, CALL, AND RE-RAISE.");
  y+=2;
  centerText(y, "MOVES");
  y++;
  y++;drawText(4,y, "FOLD - QUIT THE HAND");y++;
  y++;drawText(4,y, "CHECK- FREE PASS");y++;
  y++;drawText(4,y, "BET /- INCREASE BET. OTHERS MUST");
  y++;drawText(4,y, "RAISE  CALL TO STAY IN THE HAND");y++;
  y++;drawText(4,y, "CALL - MATCH THE CURRENT BET AND");
  y++;drawText(4,y, "       STAY IN THE HAND");
  
  centerStatusText("PRESS ANY KEY TO CONTINUE");
  drawBuffer();

  clearCommonInput();
  cgetc();
  resetScreen();
  drawBuffer();
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




bool inputFieldCycle(uint8_t x, uint8_t y, uint8_t max, uint8_t* buffer) {
  static uint8_t done, curx, lastY;
  
  // Initialize first call to input box
  //i=strlen(buffer);
  if (done == 1 || lastY != y) {
    done=0;
    lastY=y;
    curx = (unsigned char)strlen((char *)buffer);
    drawText(x,y, buffer);
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
      buffer[curx]=inputKey;
      buffer[++curx]=0;
    }

    drawText(x,y, buffer);
    drawChip(x+curx,y);

    return done==1;
  }

  return false;
  
}

void showPlayerNameScreen() {
  enableDoubleBuffer();
  resetScreenWithBorder();
  drawLogo();

  drawBuffer();
  disableDoubleBuffer();

  drawText(13,13, "ENTER YOUR NAME:");
  drawBox(15,16,PLAYER_NAME_MAX+1,1);
  drawText(16,17, playerName);
  i=strlen(playerName);
  
  clearCommonInput();
  //while (inputKey != KEY_RETURN || i<2) {
  while (!inputFieldCycle(16, 17, PLAYER_NAME_MAX, playerName)) ;
  
  enableDoubleBuffer();
  for (y=13;y<19;++y)
    drawText(13,y, "                 ");
  
  drawBuffer();

  write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, playerName);
}

/// @brief Action called in Welcome Screen to verify player has a name
void welcomeActionVerifyPlayerName() {
  // Read player's name from app key
  read_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, tempBuffer);

  tempBuffer[12]=0;
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
  i=strlen(query);
  while(--i)
    if (query[i]==' ')
      query[i]='+';
  
}

/// @brief Shows a screen to select a table to join
void showTableSelectionScreen() {
  static uint8_t shownChip;
  static unsigned char tableIndex=0;
  static uint8_t skipApiCall;
  skipApiCall=0;
  // An empty query means a table needs to be selected
  while (strlen(query)==0) {
    
    if (!skipApiCall) {
      // Show the status immediately before retrival
      centerStatusText("REFRESHING TABLE LIST..");
           //drawStatusTimer();
      drawBuffer();
      resetScreenWithBorder();
    }

      
    centerText(3, "CHOOSE A TABLE TO JOIN");
    drawText(6,6, "TABLE");
    drawText(WIDTH-13,6, "PLAYERS");
    drawLine(6,7,WIDTH-12);

    drawBuffer();
    waitvsync();

    if (/* skipApiCall || */ apiCall("tables")) {
      if (!skipApiCall) {
        updateState(true);
      }
      skipApiCall=0;
      if (tableCount>0) {
        for(i=0;i<tableCount;++i) {
          drawText(6,8+i*2, state.tables[i].name);
          drawText((unsigned char)(WIDTH-6-strlen(state.tables[i].players)), 8+i*2, state.tables[i].players);
          if (state.tables[i].players[0]>'0') {
            drawText((unsigned char)(WIDTH-6-strlen(state.tables[i].players)-2), 8+i*2, "*");
          }
        }
      } else {
        centerText(12, "SORRY, NO TABLES ARE AVAILABLE");
      }

      //drawStatusText(" R+EFRESH  H+ELP  C+OLOR  S+OUND  Q+UIT");
      drawStatusText("R-EFRESH   H-ELP  C-OLOR   N-AME   Q-UIT");
      drawBuffer();
      disableDoubleBuffer();
      shownChip=0;

      clearCommonInput();
      while (!inputTrigger || !tableCount) {
        readCommonInput();
       
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
        
        if (!shownChip || (tableCount>0 && inputDirY)) {

          drawText(4,8+tableIndex*2," ");
          tableIndex+=inputDirY;
          if (tableIndex==255) 
            tableIndex=tableCount-1;
          else if (tableIndex>=tableCount)
            tableIndex=0;

          drawChip(4,8+tableIndex*2);

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
        centerText(15, state.tables[tableIndex].name);
        
        strcpy(query, "?table=");
        strcat(query, state.tables[tableIndex].table);
        strcpy(tempBuffer, serverEndpoint);
        strcat(tempBuffer, query);

        //  Update server app key in case of reboot 
        write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, tempBuffer);

      }
    }
  }
  
  centerText(17, "CONNECTING TO SERVER");
  drawBuffer();
  
  progressAnim(19);
  
  tableActionJoinServer();
}

/// @brief Shows main game play screen (table and cards)
void showGameScreen() {
  checkIfSpectatorStatusChanged();
  checkIfPlayerCountChanged();
  
  // Animate chips to pot before drawing the new state
  animateChipsToPotOnRoundEnd();
  
  checkFinalFlip();

  resetScreen();
  resetStateIfNewGame();

  drawPot();

  if (playerCount>1) {
    drawNamePurse();
    drawBets();
    drawCards(false);
  }

  drawGameStatus();
  drawBuffer();
  highlightActivePlayer();

  prevRound = state.round;
}

/// @brief shows in-game menu
void showInGameMenuScreen() {
  hideLine(0,0,0);
  i=1;
  while (i) {
    enableDoubleBuffer();
    resetScreenWithBorder();
    drawBuffer();
    
    x = WIDTH/2-8;
    y = HEIGHT/2-4;
      
    drawBox(x-3,y-3,21,11);
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
          strcpy(query,"");
          showTableSelectionScreen();
          return;
      }
    }
  }

  // Show game screen again before returning
  resetScreen();
  drawBuffer();
  showGameScreen();
}


