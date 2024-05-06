#include "platform-specific/graphics.h"

void showInGameMenuScreen();

void centerText(unsigned char y, char * text) {
  drawText(WIDTH/2-strlen(text)/2, y, text);
}

void progressAnim(unsigned char y) {
  for(i=0;i<3;++i) {
    pause(15);
    drawChip(WIDTH/2-2+i*2,y);
    drawBuffer();
  }
}

void drawPot() {
  drawBox(WIDTH/2-3,11,3,1);
  drawChip(WIDTH/2-2,12);

  itoa(state.pot, tempBuffer, 10);
  drawText(state.pot<100 ? 20 : 19,12, tempBuffer);
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
      // Clear current player pointer
      if (state.activePlayer!=i)
        drawText(x-playerDir[i],y, " ");
      else
        drawPointer(x-playerDir[i],y);

      // Reverse print right side players
      if (playerDir[i]<0) 
        xx-=strlen(state.players[i].name)-1;

      drawText(xx, y, state.players[i].name);
      
    } else {
      drawText(x-5, playerY[i]+3, " YOU");
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
  finalFlip=cardIndex=xOffset=fullFirst=0;

  if (state.round<1)
    return;
  
  while (cardIndex <= state.round) {
    doAnim = state.round<5 && !noAnim && cardIndex >= currentCard;
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
          hand); 
        
        if (doAnim) {
          drawBuffer();
          soundDealCard();
          // for vol=2 to 0 step -1
          //   pause :sound 1,0,0,vol
          // next
          // sound
        }
      }
    }
    xOffset+=2;
    //if (xOffset>1 || (state.round==5 && !finalFlip))
    //  xOffset++;
  }

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
      soundPlayerJoin();

      // for j=8 to 2 step -2
      //   sound 1,255-j*j,10,j:pause 2:sound:pause 8
      // next
    } else {
      state.lastResult = "A NEW PLAYER JOINS THE TABLE";
      drawStatusText(state.lastResult);
      drawBuffer();
      soundPlayerLeft();
    }
  
    if (playerCount > prevPlayerCount) {
      // for j = 1 to 30 step 5
      //   sound j,20,0,1+j/5:pause 2
      //   for vol=j/5 to 0 step -1
      //     sound j,20,0,vol:pause
      //   next
      //   pause 3
      // next
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

void drawGameStatus() {
  if (state.activePlayer == 0)
    return;
  
  if (state.activePlayer>0) {
    strcpy(tempBuffer,"WAITING ON ");
    strcat(tempBuffer, state.players[state.activePlayer].name);
    drawStatusText(tempBuffer);

  } else if (state.activePlayer< 0 && (state.round == 5 || state.round == 0)) {
    //@HidePlayerSecretCardMask
    // End of (or in between) games
    if (state.round==5 && prevRound != state.round) { 
      //layMy
      // sound 1,200,10,8:pause 2
      // sound 1,170,10,8:pause 2
    }
    
    // If game over message is >40 chars, split result into two rows, rendering the first result here
    if (state.round==5 && strlen(state.lastResult)>40) { 
      //@SetStatusBarHeight 0
      // text = &lastResult$
      // FOR i=text+1 to text+peek(text)
      //   ' Split on comma (,)
      //   if peek(i) = $2C 
      //     @pos 0,24
      //     i=i-text
      //     @PrintUpper &lastResult$[1,i]
      //     @PrintSpaceRest
      //     lastResult$=lastResult$[i+2]
      //     @pos 0,25
      //     exit
      //   endif
      // NEXT
    }
    
    drawStatusText(state.lastResult);
    
    if (state.round==5 && prevRound != state.round) {
      // sound 1,150,10,8:pause 2
      // sound 1,140,10,8:pause 2
      // sound 1,135,10,8:pause 2
      // sound 1,132,10,8:pause 2
      // sound
      drawBuffer();
      soundGameDone();
    }
  }

  //@PrintSpaceRest
  if (state.round < 5) {    
    if (state.round == 0) { 
      // Waiting for a player to join
     // waitCount = (waitCount + 1) % 8;
     // @pos 30, 25
     // @PrintInv &"pop  o|po p o|po  po|p o po|p  opo|p o po|po  po|po p o"[7*waitCount+1,6]
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

    if (i == cursorX)
      drawStatusTextAt(x-1, "\"");
    
    x += 2 + strlen(state.validMoves[i].name);
  }

  // Prepare the countdown timer
  drawStatusTimeLeft();

  drawBuffer();
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
        drawStatusTextAt(moveLoc[cursorX-inputDirX]-1, " ");
        drawStatusTextAt(moveLoc[cursorX]-1, "\"");
        drawBuffer();
        soundCursor();
      } else {
        cursorX-=inputDirX;
        soundCursorInvalid();
      }

    }

    // Pressed Esc
    if (inputKey == KEY_ESCAPE || inputKey == KEY_ESCAPE_ALT) {
      showInGameMenuScreen();
      return;
    } else if (inputKey == 'h' || inputKey == 'H') {
      return;
    }
  }

  // Request the highlighted move
  if (cursorX<255) {
    requestedMove = state.validMoves[cursorX].move;
    clearStatusBar();
    drawStatusTextAt(moveLoc[cursorX], state.validMoves[cursorX].name);
    drawBuffer();
    soundSelectMove();
  }

}

void clearGameState() {
  // Reset some variables
  prevRound = 99;
  prevPlayerCount = 0;
  wasViewing = 255;
  waitCount = -1;
}

