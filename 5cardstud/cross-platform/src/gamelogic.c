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
#include "platform-specific/appkey.h"

void progressAnim(unsigned char y) {
  for(i=0;i<3;++i) {
    pause(10);
    drawChip(WIDTH/2-2+i*2,y);
    drawBuffer();
  }
}

void drawPot() {
  drawBox(WIDTH/2-3,11,4,1);
  drawChip(WIDTH/2-2,12);

  itoa(state.pot, tempBuffer, 10);
  drawText(WIDTH/2-(state.pot>99),12, tempBuffer);
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
    drawBuffer();
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
  
  for (i=0;i<playerCount;i++) {
    // Print name, left or right justified based on direction
    y = playerY[i]-1;
    x = playerX[i];
    if (playerDir[i]<0)
      x++;

    if (i>0 || state.viewing) {
      xx=x;
     
      // Reverse print right side players
      if (playerDir[i]<0) 
        xx-=strlen(state.players[i].name)-1;

      drawText(xx, y, state.players[i].name);

       // Clear current player pointer
      // if (state.activePlayer!=i)
      //   drawText(x-playerDir[i],y, " ");
      // else
      //   drawPointer(x-playerDir[i],y);
      if (state.activePlayer!=i)
        hideLine(xx, y+1, strlen(state.players[i].name));
      else {
        cursorX=xx;
        cursorY=y;
      }
    } else {
      drawText(x-5, playerY[i]+2, " YOU");
    }

    // Print purse (chip count)
    x++;
    y--;
   
    if (i==0) {
      x-=2;
      y+=3;
    }
    
    itoa(state.players[i].purse, tempBuffer, 10);

    if (playerDir[i]<0 || i==0)
      x-=strlen(tempBuffer);

    drawText(x,y, tempBuffer);
    drawChip(x-1,y);
  }
}


void drawBets() {
  if (state.round <1 || state.round>4)
    return;

  for (i=playerCount-1;i<255;i--) {
    y = playerY[i]+playerBetY[i]+1;

    // Draw bet amount
    if (state.players[i].bet>0) {
      x= playerX[i]+playerBetX[i]+1;
      
      itoa(state.players[i].bet, tempBuffer, 10);
      if (playerDir[i]<0) 
        x-=strlen(tempBuffer)+1;

      drawText(x, y, tempBuffer);
      drawChip(x-1,y );
    }

    // Draw Move
    x= playerX[i]+playerBetX[i];
    y--;
    if (playerDir[i]<0)
     x-=strlen(state.players[i].move);  
 
    drawText(x, y, state.players[i].move);

  }
}



void drawCards() {
  static bool shouldMaskPlayerCard;
  cardIndex=xOffset=fullFirst=0;
  shouldMaskPlayerCard = false;

  if (state.round<1)
    return;
  
  finalFlip = prevRound<state.round && state.round == 5;

  // Check that at least players are still in the final round, otherwise 
  // if everyone folded, the winning player does not need to flip/reveal their hand.
 
  h=0;
  for (i=0;i<playerCount;i++)
    if (state.players[i].status == 1)
      ++h;
  
  if (h<2) {
    finalFlip = false;
    shouldMaskPlayerCard = true;
  }


  while (cardIndex <= state.round) {
    doAnim = state.round<5 && !noAnim && cardIndex >= currentCard;
    if (doAnim) 
      disableDoubleBuffer();
    
    //doAnim = !noAnim && cardIndex >= currentCard;
    if (doAnim)
      pause(20);
    
    cardIndex++;
    j=cardIndex*2-1;
    
    for (h=1;h<=playerCount;h++) {
      i = h % playerCount;
      if (strlen(state.players[i].hand)>j) {
        fullFirst = i==0 && (!state.viewing || finalFlip);
     
        hand = state.players[i].hand+j-1;
        if (finalFlip && j==1 && i>0)
          hand="??";

        drawCard(
          playerX[i]+((!fullFirst)*xOffset+(fullFirst)*(j-1))*playerDir[i], 
          playerY[i],
          hand[0]!='?' || state.players[i].status != 1 || doAnim ? FULL_CARD : playerDir[i]>0 ? PARTIAL_LEFT : PARTIAL_RIGHT, 
          hand,
          j==1 && i==0 && (state.round < 5 || shouldMaskPlayerCard)); 
        
        if (doAnim) {
          soundDealCard();
          pause(cardIndex == 1 ? 10 : 18);
        }
      }
    }
    xOffset+=2;

    // Apple 2 we don't want to offset for simplicity
    //if (xOffset>1 || (state.round==5 && !finalFlip))
    //  xOffset++;
  }

  if (finalFlip) {

    drawBuffer();
    disableDoubleBuffer();
    pause(70);

    for (h=1;h<=playerCount;h++) {
      i = h % playerCount;
      if (state.players[i].status != 1)
        continue;
      
      soundTick();
      soundTick();
      drawCard(playerX[i], playerY[i], FULL_CARD, state.players[i].hand, false);
      soundDealCard();
      pause(70);
    }
  }
  
  drawBuffer();
  enableDoubleBuffer();
  currentCard = cardIndex;
  noAnim=false;
}

void checkIfSpectatorStatusChanged() {
  if (state.viewing == wasViewing)
    return;
  wasViewing = state.viewing;

  if (state.viewing) {
    drawStatusText("TABLE FULL: WATCHING AS A SPECTATOR");
    drawBuffer();
    pause(80);
  } else if (
    state.players[0].status == 0 || 
    (state.round == 1 && state.players[0].status == 1 && state.activePlayer != 0 )
    ) {
    /* Display intro text if player is joining the table on 
     * the opening round or sitting down to wait for the next round.
     * Otherwise, they are re-joining due to connection error, so we do not delay
     */

    drawStatusText("YOU SIT DOWN AT THE TABLE");
    drawBuffer();
    soundJoinGame();
    pause(50);
  }
}

void checkIfPlayerCountChanged() {
  if (playerCount == prevPlayerCount)
    return;
  
  // Handle if player joins mid game
  if (playerCount>1 && prevPlayerCount > 0) {
    if (playerCount < prevPlayerCount) {
      
      drawStatusText("A PLAYER LEFT THE TABLE");    
      drawBuffer();
      soundPlayerLeft();

      // for j=8 to 2 step -2
      //   sound 1,255-j*j,10,j:pause 2:sound:pause 8
      // next
    } else {
      state.lastResult = "A NEW PLAYER JOINS THE TABLE";
      drawStatusText(state.lastResult);
      drawBuffer();
      soundPlayerJoin();
    }
    
    pause(40);
    if (state.round > 1)
      noAnim = true;
  }

  prevPlayerCount = playerCount;
  
  // Don't shuffle player locations until multple players exist
  if (playerCount<2)
    return;
  
  i=0;
  k=(playerCount-1)*8;
  for (j=(playerCount-2)*8;j<k;j++) {
    h=playerCountIndex[j];
    playerX[i] = playerXMaster[h];
    playerY[i] = playerYMaster[h];
    playerDir[i] = playerDirMaster[h];
    playerBetX[i] = playerBetXMaster[h];
    playerBetY[i] = playerBetYMaster[h];
    i++;
  }
}

void drawStatusTimeLeft() {
  drawStatusTimer();
  tempBuffer[0]=' ';
  itoa(state.moveTime, tempBuffer+1, 10);
  drawStatusTextAt( WIDTH-strlen(tempBuffer)-2, tempBuffer);
  drawBuffer();
}

void highlightActivePlayer() {
 if (state.activePlayer>0 && playerCount>1) { 
    i=strlen(state.players[state.activePlayer].name);
    for (j=2;j<=i;j++) {
      drawLine(cursorX, cursorY+1, j);
      drawBuffer();
    }
  }
}

void animateChipsToPotOnRoundEnd() {
  if (state.round <= prevRound || state.pot == 0)
    return;

  clearStatusBar();

  // Hide moves
  for (i=0;i<playerCount;i++) {
    y = playerY[i]+playerBetY[i];
    x = playerX[i]+playerBetX[i];

    if (playerDir[i]<0)
      x-=5;  
    
    drawText(x, y, "     ");
  }

  drawBuffer();
  pause(30);

  // Don't animate bets if the pot hasn't changed
  if (prevPot == state.pot)
    return;

  prevPot = state.pot;
  disableDoubleBuffer();

  // Clear bets off screen
  for (i=0;i<playerCount;i++) {
    y = playerY[i]+playerBetY[i]+1;
    x= playerX[i]+playerBetX[i];
    
    if (playerDir[i]<0) 
      x-=3;

    drawText(x, y, "   ");
    soundTakeChip(i);
  }

  drawPot();
  drawBuffer();
  enableDoubleBuffer();

  pause(45);
}


void drawGameStatus() {
  if (state.activePlayer == 0)
    return;
  
  if (state.activePlayer>0) {
    strcpy(tempBuffer,"WAITING ON ");
    strcat(tempBuffer, state.players[state.activePlayer].name);
    drawStatusText(tempBuffer);

  } else if (state.activePlayer< 0 && (state.round == 5 || state.round == 0)) {
    // End of (or in between) games
    drawStatusText(state.lastResult);
    
    
    if (state.round==5 && prevRound != state.round) {
      drawBuffer();
      soundGameDone();
    }
  }


  if (state.round < 5) {    
    if (state.round == 0) { 
      // Waiting for a player to join
     waitCount = (waitCount + 1) % 3;
     drawText(30+waitCount,HEIGHT-2,".");
     pause(40);
    } else {
      //' Waiting on a plaer to make a move
      if (state.moveTime>0)
        drawStatusTimeLeft();
    }
  }
}

void requestPlayerMove() {
  requestedMove=NULL;

  if (state.viewing || state.activePlayer != 0)
    return;

  // Default move cursor to second position (1) if possible 
  cursorX = validMoveCount>1;

  // Draw the moves on the status bar
  clearStatusBar();
  x=1;
  for (i=0;i<validMoveCount;i++) {
    moveLoc[i] = x;
    drawStatusTextAt(x, state.validMoves[i].name);
    x += 2 + strlen(state.validMoves[i].name);
  }

  // Prepare the countdown timer
  drawStatusTimeLeft();

  drawBuffer();
  disableDoubleBuffer();

  // Zoom in the cursor
  i=strlen(state.validMoves[cursorX].name);
  h=moveLoc[cursorX];
  drawLine(0,HEIGHT-1, h+h+i);
  pause(5);

  for (j=h;j>0;--j) {
    hideLine(h-j,HEIGHT-1,1);
    hideLine(h+i+j-1,HEIGHT-1,1);
    pause(2);
  }

  soundMyTurn();


  resetTimer();

  maxJifs = 60*state.moveTime;
  waitCount=0;
  clearCommonInput();

  // Move selection loop
  while (state.moveTime>0 && !inputTrigger) {
    waitvsync();
    
    // Tick counter once per second   
    if (++waitCount>2) {
      waitCount=0;
      i = (maxJifs-getTime())/60;
      if (i!= state.moveTime) {
        state.moveTime =i;
        drawStatusTimeLeft();
        soundTick();
      }
    } 

    // Move cursor, retricting to bounds
    readCommonInput();

    if (inputDirX !=0 ) {
      cursorX+=inputDirX;
      if (cursorX<validMoveCount) {
        //drawStatusTextAt(moveLoc[cursorX-inputDirX]-1, " ");
        //drawStatusTextAt(moveLoc[cursorX]-1, "+");
        hideLine(moveLoc[cursorX-inputDirX],HEIGHT-1,strlen(state.validMoves[cursorX-inputDirX].name));
        drawLine(moveLoc[cursorX],HEIGHT-1,strlen(state.validMoves[cursorX].name));
        soundCursor();
      } else {
        cursorX-=inputDirX;
        soundCursorInvalid();
      }
      getTime();
    }

    // Pressed Esc
    switch (inputKey) {
      case KEY_ESCAPE:
      case KEY_ESCAPE_ALT:
        showInGameMenuScreen();
        return;
    }
  }
  clearStatusBar();
  enableDoubleBuffer();

  // Request the highlighted move
  if (cursorX<255) {
    requestedMove = state.validMoves[cursorX].move;
    clearStatusBar();
    drawStatusTextAt(moveLoc[cursorX], state.validMoves[cursorX].name);
    drawBuffer();
    soundSelectMove();

    // For some unknown reason, if I don't delay for a bit here, 
    // the selectMove sound plays twice in the AppleWin emulator.
    pause(30); 
    
  }

}

void clearGameState() {
  // Reset some variables
  prevRound = 99;
  prevPlayerCount = 0;
  wasViewing = 255;
  waitCount = -1;
}

