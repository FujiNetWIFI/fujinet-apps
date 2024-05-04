#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <conio.h>
#endif /* _CMOC_VERSION_ */

/// @brief Convenience function to reset screen and draw border
void resetScreenWithBorder() {
  resetScreen();
  drawBorder();
}

/// @brief Shows information about the game
void showHelpScreen() {
  // This COULD be retrieved from the server, especially if
  // this client were game agnostic.
  resetScreenWithBorder();
  
  centerText(3,"HOW TO PLAY 5 CARD STUD");
  y=4;
                   // __________________________________
  y++;drawText(3,y, "PLAYERS ARE DEALT 5 CARDS OVER THE");
  y++;drawText(3,y, "COURSE OF 4 ROUNDS. ON EACH ROUND");
  y++;drawText(3,y, "PLAYERS BET, CALL, AND RE-RAISE.");
  y+=2;
  centerText(y, "MOVES");
  y++;
  y++;drawText(3,y, "FOLD   - QUIT THE HAND");y++;
  y++;drawText(3,y, "CHECK  - FREE PASS");y++;
  y++;drawText(3,y, "BET OR - INCREASE BET. OTHERS MUST");
  y++;drawText(3,y, "RAISE    CALL TO STAY IN THE HAND");y++;
  y++;drawText(3,y, "CALL   - MATCH THE CURRENT BET AND");
  y++;drawText(3,y, "         STAY IN THE HAND");
  
  drawBuffer();
  drawStatusTextAt(WIDTH/2-12, "PRESS ANY KEY TO CONTINUE");
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
  
  i=strlen(tempBuffer);
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

/// @brief Action called in Welcome Screen to verify player has a name
void welcomeActionVerifyPlayerName() {
  // Read player's name from app key
  
  read_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, playerName);
 
    
    strcpy(playerName,"ERICAPL2");

  // Quick and dirty input if they didn't come in from the lobby
  if (strlen(playerName) == 0) {
    drawBuffer();
    disableDoubleBuffer();
    drawText(13,12, "ENTER YOUR NAME: ");
    drawText(13,13, "8 LETTERS OR LESS");
    
    drawBox(15,16,8,1);
    drawText(16,17,"");

    // 
    cscanf("%8s", playerName);
    
    enableDoubleBuffer();
    
    write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, playerName);
  }
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
  
  // If first run, show the help screen
  read_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_SHOWHELP, tempBuffer);
  
  if (strlen(tempBuffer)==0) {
    strcpy(tempBuffer,"1");
    write_appkey(AK_CREATOR_ID, AK_APP_ID, AK_KEY_SHOWHELP, tempBuffer);
    pause(60);
    showHelpScreen();
  }
  //pause(30);
} 

/// @brief Action in Table Selection Screen, joins the selected server
void tableActionJoinServer() {
  // Reset the game state
  clearGameState();
  
  strcat(query,"&player=");
  strcat(query,playerName);
  
  // Replace space with + for player name
  i=strlen(query);
  while(--i)
    if (query[i]==' ' || query[i]=='&')
      query[i]='+';
  
}

/// @brief Shows a screen to select a table to join
void showTableSelectionScreen() {
  static unsigned char tableIndex=0;
  
  // An empty query means a table needs to be selected
  while (strlen(query)==0) {

    // Show the status immediately before retrival
    drawStatusText("REFRESHING TABLE LIST");
    drawStatusTimer();
    drawBuffer();

    resetScreen();
    drawBorder();
    centerText(4, "CHOOSE A TABLE TO JOIN");
    drawText(3,7, "TABLE");
    drawText(WIDTH-7-3,7, "PLAYERS");

    drawLine(3,8,WIDTH-6);

    if (apiCall("tables")) {
      updateState();

      if (tableCount>0) {
        for(i=0;i<tableCount;++i) {
          drawText(3,9+i*2, state.tables[i].name);
          drawText(WIDTH-3-strlen(state.tables[i].players), 9+i*2, state.tables[i].players);
        }
      } else {
        centerText(12, "SORRY, NO TABLES ARE AVAILABLE");
      }

      drawStatusText("R-REFRESH    H-HELP    C-COLOR    Q-QUIT");
      drawBuffer();

      clearCommonInput();
      while (!inputTrigger || !tableCount) {
        readCommonInput();
      
        if (inputKey == 'h') {
          showHelpScreen();
          break;
        } else if (inputKey == 'r' || inputKey == 'R') {
          break;
        } else if (inputKey == 'q') {
          quit();
        } else if (inputKey != 0) {
          itoa(inputKey, tempBuffer, 10);
          drawStatusText(tempBuffer);
        }

        if (tableCount>0) {
          drawBlank(2,9+tableIndex*2);
          tableIndex+=inputDirY;
          if (tableIndex==255) 
            tableIndex=tableCount-1;
          else if (tableIndex>=tableCount)
            tableIndex=0;

          drawChip(2,9+tableIndex*2);
          drawBuffer();
        }
      }

      if (inputTrigger) {
        strcpy(query, "?table=");
        strcat(query, state.tables[tableIndex].table);

        strcpy(tempBuffer, serverEndpoint);
        strcat(tempBuffer, query);

        //  Update server app key in case of reboot 
        write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, tempBuffer);

        // Clear screen and write server name
        resetScreenWithBorder();
        centerText(15, state.tables[tableIndex].name);
      }
    }
  }
  
  centerText(18, "CONNECTING TO SERVER");
  drawBuffer();
  
  progressAnim(20);
  
  //tableActionJoinServer();
}

/// @brief Shows main game play screen (table and cards)
void showGameScreen() {
  checkIfSpectatorStatusChanged();
  checkIfPlayerCountChanged();
  // TODO animateChipsToPotOnRoundEnd();
  
  resetScreen();
  resetStateIfNewGame();

  drawPot();

  if (playerCount>1) {
    drawNamePurse();
    drawBets();
    drawCards();
  }

  drawGameStatus();
  //highlightActivePlayer();

  drawBuffer();
  prevRound = state.round;
}

/// @brief shows in-game menu
void showInGameMenuScreen() {
  resetScreenWithBorder();
  
  x = WIDTH/2-8;
  y = HEIGHT/2-4;
  
  drawBox(x-3,y-2,20,9);
  drawText(x,y,    "  Q: QUIT TABLE");
  drawText(x,y+=2, "  H: HOW TO PLAY"); 
  drawText(x,y+=2, "  C: TABLE COLOR"); 
  drawText(x,y+=2, "ESC: KEEP PLAYING"); 

  drawBuffer();

  clearCommonInput();
  while (true) {
    readCommonInput();
    switch (inputKey) {
      case 'h':
        showHelpScreen();
      case 3: // Esc
     
        // Show game screen again before returning
        resetScreen();
        drawBuffer();
        showGameScreen();
        return;

      case 'q':
        resetScreenWithBorder();
        centerText(12, "PLEASE WAIT");
        drawBuffer();

        // Inform server player is leaving
        apiCall("leave");
        progressAnim(14);
        
        //  Clear server app key in case of reboot 
        write_appkey(AK_LOBBY_CREATOR_ID,  AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, "");

        // Clear query so a new table will be selected
        strcpy(query,"");
        showTableSelectionScreen();
        return;
    }
  }
}


