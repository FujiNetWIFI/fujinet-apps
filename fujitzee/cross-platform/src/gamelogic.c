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

void drawPot() {
  
}

void resetStateIfNewGame() {
  if (state.round >= prevRound)
    return;

  // Reset status bar and vars for a new game
  if (prevRound != 99) {
   // @SetStatusBarHeight 1  
   clearStatusBar(); 
  } else {

    // Force empty screen if coming from another screen.
    // This is mainly to avoid color glitches on c64
    // when setting the color memory - which is not (YET?) double buffered -
    // before the screen is drawn. There may be a better solution.
    
  }

  xOffset=0;
  currentCard=0;
  cursorY=246;
  cursorX=128;
  prevPot=0;
  
  // If the round is already past 1, we are joining a game in progress. Skip animation this update
  if (state.round>1)
    noAnim=1;
}


void drawNamePurse() {
  
}


void drawBets() {
  
}

void checkFinalFlip() {
 
}

void drawCards(bool finalFlip) {
  
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

void requestPlayerMove() {
  requestedMove=NULL;

  if (state.viewing || state.activePlayer != 0)
    return;

}

void clearGameState() {
  // Reset some variables
  prevRound = 99;
  prevPlayerCount = 0;
  wasViewing = 255;
  waitCount = -1;
}

void handleChat() {
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