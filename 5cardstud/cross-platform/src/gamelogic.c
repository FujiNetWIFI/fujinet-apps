#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
//#define true 1
//#define false 0
//typedef BOOL bool;
#else
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif /* _CMOC_VERSION_ */

#include "platform-specific/graphics.h"
#include "platform-specific/sound.h"
#include "platform-specific/util.h"
#include "gamelogic.h"
#include "misc.h"
#include "stateclient.h"
#include "screens.h"
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
  
  for (i=0;i<state.playerCount;i++) {
    // Print name, left or right justified based on direction
    y = playerY[i]-1;
    x = playerX[i];
    if (playerDir[i]<0)
      x++;

    if (i>0 || state.viewing) {
      xx=x;
     
      // Reverse print right side players
      if (playerDir[i]<0) 
        xx-=(unsigned char)strlen(state.players[i].name)-1;

      drawText(xx, y, state.players[i].name);

      if (state.activePlayer!=i) {
        hideLine(xx, y+1, (unsigned char)strlen(state.players[i].name));
      } else {
        cursorX=xx;
        cursorY=y;
      }
    } else {
      drawText(x-5, playerY[i]+2, (const char *)" YOU");
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
      x-=(unsigned char)strlen(tempBuffer);

    drawText(x,y, tempBuffer);
    drawChip(x-1,y);
  }
}


void drawBets() {
  if (state.round <1 || state.round>4)
    return;

  for (i=state.playerCount-1;i<255;i--) {
    y = playerY[i]+playerBetY[i]+1;

    // Draw bet amount
    if (state.players[i].bet>0) {
      x= playerX[i]+playerBetX[i]+1;
      
      itoa(state.players[i].bet, tempBuffer, 10);
      if (playerDir[i]<0) 
        x-=(unsigned char)strlen(tempBuffer)+1;

      drawText(x, y, tempBuffer);
      drawChip(x-1,y );
    }

    // Draw Move
    x= playerX[i]+playerBetX[i];
    y--;
    if (playerDir[i]<0)
      x-=(unsigned char)strlen(state.players[i].move);  
 
    drawText(x, y, state.players[i].move);

  }
}

void checkFinalFlip() {
  if (prevRound<state.round && state.round == 5)
    drawCards(true);
}

void drawCards(bool finalFlip) {
  static bool shouldMaskPlayerCard, doNotFlipCards;
  cardIndex=xOffset=fullFirst=doNotFlipCards=0;
  shouldMaskPlayerCard = true;

  if (state.round<1)
    return;
  
  
  // Check that at least players are still in the final round, otherwise 
  // if everyone folded, the winning player does not need to flip/reveal their hand.
  if (state.round == 5) {
    h=0;
    for (i=0;i<state.playerCount;i++)
      if (state.players[i].status == 1)
        ++h;
    
    if (h<2) {
      finalFlip = false;
      shouldMaskPlayerCard = true;
      doNotFlipCards=true;
    } else if (!finalFlip) {
      shouldMaskPlayerCard = false;
    }
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
    
    for (h=1;h<=state.playerCount;h++) {
      i = h % state.playerCount;
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
#ifndef DISABLE_SOUND    
          soundDealCard();
#endif          
          pause(cardIndex == 1 ? 1 : 10);
        }
      }
    }

    xOffset++;

    // Apple 2 doesn't render half cards correctly - rather, the offset cards switch colors, so always_render_full_cards is true for now
    if (always_render_full_cards || xOffset>1 || (state.round==5 && !finalFlip && !doNotFlipCards))
      xOffset++;
  }

  if (finalFlip) {

    drawBuffer();
    disableDoubleBuffer();

    for (h=1;h<=state.playerCount;h++) {
      i = h % state.playerCount;

      // Don't flip a player that doesn't have a visible hand
      if (state.players[i].status != 1 || state.players[i].hand[0]=='?')
        continue;
      
      for (j=1;j<=9; j+=2) {
        drawCard(playerX[i]+(j-1)*playerDir[i], playerY[i], FULL_CARD,state.players[i].hand+j-1, false);
      }
#ifndef DISABLE_SOUND    
      soundDealCard();
#endif      
      pause(35);
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
#ifndef DISABLE_SOUND    
    soundJoinGame();
#endif    
    pause(50);
  }
}

void checkIfPlayerCountChanged() {
  if (state.playerCount == prevPlayerCount)
    return;
  
  // Handle if player joins mid game
  if (state.playerCount>1 && prevPlayerCount > 0) {
    if (state.playerCount < prevPlayerCount) {
      
      drawStatusText("A PLAYER LEFT THE TABLE");    
      drawBuffer();
#ifndef DISABLE_SOUND    
      soundPlayerLeft();
#endif
      // for j=8 to 2 step -2
      //   sound 1,255-j*j,10,j:pause 2:sound:pause 8
      // next
    } else {
      strcpy(state.lastResult, "A NEW PLAYER JOINS THE TABLE");
      drawStatusText(state.lastResult);
      drawBuffer();
#ifndef DISABLE_SOUND    
      soundPlayerJoin();
#endif      
    }
    
    pause(40);
    if (state.round > 1)
      noAnim = true;
  }

  prevPlayerCount = state.playerCount;
  
  // Don't shuffle player locations until multple players exist
  if (state.playerCount<2)
    return;
  
  i=0;
  k=(state.playerCount-1)*8;
  for (j=(state.playerCount-2)*8;j<k;j++) {
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
  drawStatusTextAt( (unsigned char)(WIDTH-strlen(tempBuffer)-2), tempBuffer);
  drawBuffer();
}

void highlightActivePlayer() {
 if (state.activePlayer>0 && state.playerCount>1) { 
   i=(unsigned char)strlen(state.players[state.activePlayer].name);
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
  pause(50);

  // Hide moves
  for (i=0;i<state.playerCount;i++) {
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
  for (i=0;i<state.playerCount;i++) {
    y = playerY[i]+playerBetY[i]+1;
    x= playerX[i]+playerBetX[i];
    
    if (playerDir[i]<0) 
      x-=3;

    drawText(x, y, "   ");
#ifndef DISABLE_SOUND    
    soundTakeChip(i);
#endif    
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
#ifndef DISABLE_SOUND    
      soundGameDone();
#endif      
      pause(30);
    }
  }


  // Waiting for a player to join - show animation
  if (state.round == 0) {  
    drawStatusTextAt(30+waitCount," ");
    waitCount = (waitCount + 1) % 3;
    drawStatusTextAt(30+waitCount,".");
    pause(40);
  } 

}

void requestPlayerMove() {
  requestedMove=NULL;

  if (state.viewing || state.activePlayer != 0)
    return;

  // Default move cursor to second position (1) if possible 
  cursorX = state.validMoveCount>1;

  // Draw the moves on the status bar
  clearStatusBar();
  x=1;
  for (i=0;i<state.validMoveCount;i++) {
    moveLoc[i] = x;
    drawStatusTextAt(x, state.validMoves[i].name);
    x += 2 + (unsigned char)strlen(state.validMoves[i].name);
  }

  // Prepare the countdown timer
  drawStatusTimeLeft();

  drawBuffer();
  disableDoubleBuffer();

  // Zoom in the cursor
  i=(unsigned char)strlen(state.validMoves[cursorX].name);
  h=moveLoc[cursorX];
 


// Animate the line. If needed, can add #if around instead
// of checking render full cards
//if (always_render_full_cards) {
  drawLine(0,HEIGHT-1, h+h+i);
  pause(5);
  for (j=h;j>0;--j) {
    hideLine(h-j,HEIGHT-1,1);
    hideLine(h+i+j-1,HEIGHT-1,1);
    pause(2);
  }
//}

  drawLine(h,HEIGHT-1,i);

#ifndef DISABLE_SOUND    
  soundMyTurn();
#endif

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
      i = (unsigned char)((maxJifs-getTime())/60);
      if (i!= state.moveTime) {
        state.moveTime =i;
        drawStatusTimeLeft();
#ifndef DISABLE_SOUND    
        soundTick();
#endif        
      }
    } 

    // Move cursor, retricting to bounds
    readCommonInput();

    if (inputDirX !=0 ) {
      cursorX+=inputDirX;
      if (cursorX<state.validMoveCount) {
        //drawStatusTextAt(moveLoc[cursorX-inputDirX]-1, " ");
        //drawStatusTextAt(moveLoc[cursorX]-1, "+");
       
        hideLine(moveLoc[cursorX-inputDirX],HEIGHT-1,(unsigned char)strlen(state.validMoves[cursorX-inputDirX].name));
        drawLine(moveLoc[cursorX],HEIGHT-1,(unsigned char)strlen(state.validMoves[cursorX].name));
#ifndef DISABLE_SOUND    
        soundCursor();
#endif        
      } else {
        cursorX-=inputDirX;
#ifndef DISABLE_SOUND    
        soundCursorInvalid();
#endif        
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
#ifndef DISABLE_SOUND    
    soundSelectMove();
#endif    

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

