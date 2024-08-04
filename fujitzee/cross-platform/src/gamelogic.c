#include <stdlib.h>
#include <string.h>
#include "platform-specific/graphics.h"
#include "platform-specific/sound.h"
#include "platform-specific/util.h"
#include "gamelogic.h"
#include "misc.h"
#include "stateclient.h"
#include "screens.h"
#include <stdio.h>

uint8_t chat[20]="";

void progressAnim(unsigned char y) {
  for(i=0;i<3;++i) {
    pause(10);
    drawChip(WIDTH/2-2+i*2,y);
  }
}


void processStateChange() {

  renderBoardNamesMessages();
  
  // // Animate chips to pot before drawing the new state
  // animateChipsToPotOnRoundEnd();
  

  // resetScreen();
  // resetStateIfNewGame();

  // drawPot();

  // if (playerCount>1) {
  //   drawNamePurse();
  //   drawBets();
  //   drawCards(false);
  // }

  // drawGameStatus();
  // 
  // highlightActivePlayer();

  prevRound = state.round;
}


void renderBoardNamesMessages() {
  static uint8_t i;

  // Draw board if we haven't drawn it yet
  if (prevRound == 99) {
    drawBoard();

      // Player ready indicators if waiting to start
    if (state.round == 0) {
      centerTextAlt(HEIGHT-1,"press TRIGGER/SPACE to toggle");
    }
  }

  // Display prompt if changed in ready mode
  if (state.round == 0 && promptChanged) {
     
      centerTextWide(HEIGHT-3,state.prompt); // "waiting for everyone to ready up."
      promptChanged = false;
  }

  // Draw player names if the count changed
  if (playerCount != prevPlayerCount) {
    for(i=0;i<6;i++) {
      if (i<playerCount) {
        // Player name list
        drawTextAlt(1,i+1,state.players[i].alias);
        drawText(2,i+1,&state.players[i].name[1]);

        // Player initials across top of screen
        drawTextAlt(18+i*4,0,state.players[i].alias);
      } else if (i<prevPlayerCount) {
        drawText(1,i+1,"        ");
      }
    }

    prevPlayerCount=playerCount;
  }

  // Show players that are ready to start
  if (forceReadyUpdates && state.round == 0) {
    for(i=0;i<6;i++) {
      if (i<playerCount && state.players[i].scores[0]) {
       drawTextVert(18+i*4,2,"ready");
       drawChip(0,i+1);
      } else {
       drawTextVert(18+i*4,2,"     ");
       drawBlank(0,i+1);
      }
    }

    forceReadyUpdates = 0;
  }

}



void drawPot() {
  
}

void resetStateIfNewGame() {
  if (state.round >= prevRound)
    return;

  // Reset status bar and vars for a new game
  if (prevRound != 99) {
   // @SetStatusBarHeight 1  
   //clearStatusBar(); 
  } else {

    // Force empty screen if coming from another screen.
    // This is mainly to avoid color glitches on c64
    // when setting the color memory - which is not (YET?) double buffered -
    // before the screen is drawn. There may be a better solution.
    
  }

  cursorX=128;
  
  // If the round is already past 1, we are joining a game in progress. Skip animation this update
  if (state.round>1)
    noAnim=1;
}


void drawNamePurse() {
  
}


void drawBets() {
  
}


void checkIfSpectatorStatusChanged() {
  // if (state.viewing == wasViewing)
  //   return;
  // wasViewing = state.viewing;

  // if (state.viewing) {
  //  // drawStatusText("game full: watching as spectator");
    
  //   pause(80);
  // } else if (
  //   state.players[0].status == 0 || 
  //   (state.round == 1 && state.players[0].status == 1 && state.activePlayer != 0 )
  //   ) {
  //   /* Display intro text if player is joining the table on 
  //    * the opening round or sitting down to wait for the next round.
  //    * Otherwise, they are re-joining due to connection error, so we do not delay
  //    */

  //  // drawStatusText("YOU SIT DOWN AT THE TABLE");
    
  //   soundJoinGame();
  //   pause(50);
  // }
}

void checkIfPlayerCountChanged() {
  if (playerCount == prevPlayerCount)
    return;

}

void drawStatusTimeLeft() {

}

void highlightActivePlayer() {
 
}


void drawGameStatus() {
  
}

void clearGameState() {
  // Reset some variables
  wasViewing = 255;
  waitCount = -1;
  requestedMove=NULL;
  forceRender();
}


// Invalidate state variables that will trigger re-rendering of screen items
void forceRender() {
  prevRound = 99;
  prevPlayerCount = 0;
  forceReadyUpdates = 1;
}


/// @brief Convenience function to draw text centered at row Y
void centerText(unsigned char y, char * text) {
  drawText(WIDTH/2-strlen(text)/2, y, text);
}

/// @brief Convenience function to draw text centered at row Y, blanking out the rest of the row
void centerTextWide(unsigned char y, char * text) {
  i = strlen(text);
  x = WIDTH/2-i/2;

  drawSpace(0,y, x);
  drawText(x, y, text);
  drawSpace(x+i,y, WIDTH-x-i);
}

/// @brief Convenience function to draw text centered at row Y in alternate color
void centerTextAlt(unsigned char y, char * text) {
  drawTextAlt(WIDTH/2-strlen(text)/2, y, text);
}

/// @brief Convenience function to draw status text centered
void centerStatusText(char * text) {
  drawTextAlt((WIDTH-strlen(text))>>1,HEIGHT-1,text);
}


/// @brief Handles available key strokes for the defined input box (player name and chat). Returns true if user hits enter
bool inputFieldCycle(uint8_t x, uint8_t y, uint8_t max, uint8_t* buffer) {
  static uint8_t done, curx, lastY;
  
  // Initialize first call to input box
  //i=strlen(buffer);
  if (done == 1 || lastY != y) {
    done=0;
    lastY=y;
    curx = strlen(buffer);
    drawTextAlt(x,y, buffer);
    drawTextCursor(x+curx,y);
  }
   // curx=i;
   // done=0;
 // }




  // Process any waiting keystrokes
  if (kbhit()) {
    done=0;
    
    inputKey = cgetc();

    // Debugging - See what key was pressed
    // itoa(inputKey, tempBuffer, 10);drawText(0,0, tempBuffer);

    if (inputKey == KEY_RETURN && curx>1) {
      done=1;
      // remove cursor
      drawText(x+1+i,y," ");
    } else if (inputKey == KEY_BACKSPACE && curx>0) {
      buffer[--curx]=0;
      drawText(x+1+curx,y," ");
    } else if (
      curx < max && ((curx>0 && inputKey == KEY_SPACEBAR) || (inputKey>= 48 && inputKey <=57) || (inputKey>= 65 && inputKey <=90) || (inputKey>= 97 && inputKey <=122))    // 0-9 A-Z a-z
    ) {
      
      if (inputKey>=65 && inputKey<=90)
        inputKey+=32;

      buffer[curx]=inputKey;
      buffer[++curx]=0;
    }

    drawTextAlt(x,y, buffer);
    drawTextCursor(x+curx,y);

    return done==1;
  }

  return false;
  
}

void processInput() {
  readCommonInput();

  //if (state.viewing || state.activePlayer != 0)
    
  if (!state.viewing) {
    // Toggle readiness if waiting to start game
    if (state.round == 0 && inputTrigger) {
      state.players[0].scores[0] = !state.players[0].scores[0];
      forceReadyUpdates=true;
      renderBoardNamesMessages();
      sendMove("ready");
      return;
    }
  }
  switch(inputKey) {
      case KEY_ESCAPE: // Esc
      case KEY_ESCAPE_ALT: // Esc Alt
        showInGameMenuScreen();  
        break;
    }    

  // static bool chatInit=false;
  // if (kbhit()) {
  //   //if (!chatInit) {
  //   //  drawText(20,23,">>");
  //   //  inputFieldCycle(22,23,18, chat);
  //     //chat[0]=0;
  //     //chatInit=true;
  //   //}

    
  // }
  //readCommonInput();
}

