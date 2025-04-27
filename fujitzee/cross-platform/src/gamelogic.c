#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
// Fill in for peekpoke.h (comes from cc65 on other platforms)
#define POKE(addr,val)     (*(unsigned char*) (addr) = ((unsigned char)(val)))
#define POKEW(addr,val)    (*(unsigned*) (addr) = ((unsigned)(val)))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))
#else
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef __ADAM__
#include <peekpoke.h>
#endif /* __ADAM__ */
#endif
#include "platform-specific/graphics.h"
#include "platform-specific/sound.h"
#include "platform-specific/util.h"
#include "gamelogic.h"
#include "misc.h"
#include "stateclient.h"
#include "screens.h"

uint8_t chat[20]="";
uint8_t scoreY[] = {1,2,3,4,5,6,8,9,11,12,13,14,15,16,17,19};
char* scores[]={"one","two","three","four","five","six","total","bonus","set 3","set 4","house","s run","l run","count","","SCORE"};
uint8_t prevCursorX, cursorXMin;

void progressAnim(unsigned char y) {
  for(i=0;i<3;++i) {
    pause(10);
    drawChip(WIDTH/2-2+i*2,y);
  }
}

void processStateChange() {

  renderBoardNamesMessages();
  handleAnimation();

  state.prevActivePlayer = state.activePlayer;
  state.prevRollsLeft = state.rollsLeft;
  prevRound = state.round;
}


void renderBoardNamesMessages() {
  static bool redraw;
  static uint8_t scoreCursorX, scoreCursorY;

  redraw = state.round < prevRound;
  
  // Draw board if we haven't drawn it yet, or a new game
  if (redraw) {
    drawBoard();

      // Player ready indicators if waiting to start
    if (state.round == 0) {
      centerTextAlt(HEIGHT-1,"press TRIGGER/SPACE to toggle");
    }
  }

  // Draw player names if the count changed
  if (redraw || playerCount != prevPlayerCount) {
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

  // Round 0 (waiting to start) checks, or going into round 1
  if (state.round ==0 || (state.round == 1 && prevRound==0)) {
    // Display "waiting for players" prompt if changed in ready mode
    if (state.round==0 && promptChanged) {
        centerTextWide(HEIGHT-3,state.prompt);
        promptChanged = false;
    }

    // Show players that are ready to start
    if (forceReadyUpdates) {
      for(i=0;i<6;i++) {
        if (i<playerCount && state.players[i].scores[0]==1) {
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

  // Exit early as below text is for rounds > 0 
  if (state.round == 0)
    return;
    
  // Indicate active player if changed
  if (state.activePlayer != state.prevActivePlayer) {

    if (state.round == 99)
      drawSpace(0,HEIGHT-5,200);

    // Remove score cursor if player was last to play
    if (state.prevActivePlayer==0 && cursorX>=10)
      drawBlank(17,scoreY[cursorX-10]);

    // Update scores on-screen - two pass - first highlights in green the new one
    h = 0;
    scoreCursorY=0;
    for (k=0;k<2;k++) {
      for (i=0;i<playerCount;i++) {
        for (j=0;j<16;j++) {
          if (state.players[i].scores[j]>-1) {
            itoa(state.players[i].scores[j], tempBuffer, 10);
            if (j==15 || (k==0 && i>0 && state.activePlayer>-1 && isEmpty(19+i*4, scoreY[j]))) {
              drawTextAlt(20-strlen(tempBuffer)+i*4,scoreY[j],tempBuffer);
              if (scoreCursorY==0 ) {
                scoreCursorY=scoreY[j];
                scoreCursorX=17+i*4;
              }
              h++;
            } else {
              drawText(20-strlen(tempBuffer)+i*4,scoreY[j],tempBuffer);
            }
          }
        }
        if (k==0 && state.round<99 && scoreCursorY>0 && h>0 && h<5) {
          drawScoreCursor(scoreCursorX,scoreCursorY);
          soundScore();
        }
      }

      // Pause unless a ton of numbers changed
      if (scoreCursorY>0 && h>0 && h<5) {
        pause(30);
        drawBlank(scoreCursorX,scoreCursorY);
      }
    }

  
     // Clear old turn indicator
    if (state.prevActivePlayer>-1)
      drawBlank(0,state.prevActivePlayer+1);
    
    // Draw new active player indicator
    if (state.activePlayer>-1)
      drawChip(0,state.activePlayer+1);

    // Handle end of game
    if (state.round == 99) { 
      centerText(HEIGHT-3, state.prompt);
      soundGameDone();

      pause(state.moveTime*60);
      centerTextAlt(HEIGHT-1,"press TRIGGER/SPACE to continue");
      clearCommonInput();
      while (!inputTrigger) {
        readCommonInput();
        waitvsync();
      }
      drawSpace(0,HEIGHT-5,200);
     
    } else {
      pause(30);
      drawSpace(0,HEIGHT-5,200);
      drawText(0,HEIGHT-3-(state.activePlayer==0), state.prompt);
      pause(30);
    }

    if (state.activePlayer == 0)
      soundMyTurn();
  }

}

void handleAnimation() {
  static bool isThisPlayer;
  static uint8_t highScoreIndex;
  static int16_t score;

  waitvsync();  
  isThisPlayer = state.activePlayer==0;

  // Setup the player input details if this is a new roll
  if (state.rollsLeft != state.prevRollsLeft || state.activePlayer != state.prevActivePlayer )  {
    state.rollFrames=31;
    if (isThisPlayer) {
      state.playerMadeMove=false;
      prevCursorX=5;
      cursorX=1;

      // If no rolls are remaining, default the cursor on the highest score
      if (state.rollsLeft==0) {
        highScoreIndex = 0;
        for (j=1;j<15;j++) {
          if (state.validScores[j]>state.validScores[highScoreIndex])
            highScoreIndex = j;
        }
        cursorX=10+highScoreIndex;
      }
    }
  }

  if (state.activePlayer<0 || !state.rollFrames)
    return;

  state.rollFrames--;
  

  // Draw the dice, randomly displaying the ones that are currently being rolled
  if (state.rollFrames % 4==0)
    soundRollDice();

  for(i=0;i<5;i++) {
    if (state.rollFrames && state.keepRoll[i]=='1' ) {
      // Draw a random die
      drawDie(20+4*i,HEIGHT-4,rand()%6+1,0);
    } else {
      // Draw the kept die
      drawDie(20+4*i,HEIGHT-4,state.dice[i]-48,(state.rollFrames || state.rollsLeft>0) && state.keepRoll[i]=='0');
    }
  }

  soundStop();

  // If the rolling has stopped and this player is playing
  if (isThisPlayer && !state.rollFrames) {

    // Display the potential scores for this player
    for (j=0;j<15;j++) {
      score= state.validScores[j];
      if (score>=0) {
        if (score>0) {
          itoa(score, tempBuffer, 10);
          drawSpace(17,scoreY[j],3-strlen(tempBuffer)); 
        }
        else
          strcpy(tempBuffer,"  ");
        drawTextAlt(20-strlen(tempBuffer),scoreY[j],tempBuffer);
      } 
    }

    // Draw "Rolls" die if this player's turn and there are rolls left
    drawDie(15,HEIGHT-4,state.rollsLeft+13,0);
  }
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
      if (state.players[0].scores[0]) 
        soundScore();
      else
        soundCursorScore();

      sendMove("ready");
      return;
    }

    // Wait on this player to make roll decisions
    if (!state.rollFrames && state.activePlayer == 0 && !state.playerMadeMove) {
      waitOnPlayerMove();
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

void waitOnPlayerMove() {
  static int jifsPerSecond, seconds;
  static bool foundValidLocation;
  
  resetTimer();

  // Determine max jiffies for PAL and NTS
#ifdef __ATARI__
  jifsPerSecond=PEEK(0xD014)==1 ? 50 : 60;
#endif /* __ATARI__ */
  maxJifs = jifsPerSecond*state.moveTime;
  waitCount=0;
  
  // Move selection loop
  while (state.moveTime>0) {
   
      // Update horizontal cursor location - dice: 0 = roll, 1-5 equal select dice at that index
      if (inputDirX !=0 && state.rollsLeft) {
        // If cursor was selecting a score, bring it back down
        if (cursorX>9)
          cursorX = 1;

        cursorX+=inputDirX;

        // Bounds check
        if (cursorX<0 || cursorX>5)
          cursorX = prevCursorX;
      
      // Update vertical cursor location - scores: 10-24 represent the possible scoring locations
      } else if (inputDirY != 0) {
        // If cursor was selecting dice, bring it up
        if (cursorX<10)
          cursorX=25;
        
        while (1) {
          cursorX+=inputDirY;
          
          // Bounds checks
          foundValidLocation=true;
          if (cursorX<10 || (cursorX>24 && state.rollsLeft==0))
            cursorX = prevCursorX;
          else if (cursorX>24)
            cursorX=prevCursorX <10 ? prevCursorX : 1;
          else {
            // Skip over invalid scores
            if (state.validScores[cursorX-10]<0)
              foundValidLocation=false;
          }

          // Break once we are at a valid location
          if (foundValidLocation)
            break;
        }
      }
     
    waitvsync();

    // Draw cursor
    if (cursorX != prevCursorX) {
      
      // Hide cursor
      if (prevCursorX < 6)
        hideCursor(4*prevCursorX-(prevCursorX==0)+16, HEIGHT-4);
      else {
        drawBlank(17,scoreY[prevCursorX-10]);
        if (state.validScores[prevCursorX-10]==0) {
          drawBlank(19,scoreY[prevCursorX-10]);
        }
      }

      // Draw cursor
      if (cursorX < 6) {
        drawCursor(4*cursorX-(cursorX==0)+16, HEIGHT-4);
        soundCursor();
      } else {
        drawScoreCursor(17,scoreY[cursorX-10]);
        if (state.validScores[cursorX-10]==0) {
          drawTextAlt(19,scoreY[cursorX-10],"0");
        }
        soundCursorScore();
      }
      prevCursorX = cursorX;
      
      
    }

    // Handle trigger press
    if (inputTrigger) {
      
      if (cursorX>=10) {
        // Select score
        i=cursorX-10;

        // First, hide all scores but the main score
        for (j=0;j<15;j++) {
          if (state.validScores[j]>0) {
            if (j != i)
              drawSpace(17,scoreY[j],3);
          }
        }
        
        // Send command to score this value
        strcpy(tempBuffer, "score/");
        itoa(i, tempBuffer+6, 10);
        sendMove(tempBuffer);

        state.playerMadeMove = true;
        soundScore();
        return;
      } else if (cursorX>0) {
        // Toggle kept state of die
        i = state.keepRoll[cursorX-1]= state.keepRoll[cursorX-1]=='1' ? '0' : '1';
        drawDie(16+4*cursorX,HEIGHT-4,state.dice[cursorX-1]-48,i == '0');
        if (i=='0')
          soundKeep();
        else 
          soundRelease();
      } else {
        // Request another roll

        // Highlight the roll die in green
        drawDie(15,HEIGHT-4,state.rollsLeft+15,0);
        
        strcpy(tempBuffer, "roll/");
        strcat(tempBuffer, state.keepRoll);
        sendMove(tempBuffer);

        state.playerMadeMove = true;
        hideCursor(4*prevCursorX-(prevCursorX==0)+16, HEIGHT-4);

        return;
      }
    }
    
    // Tick counter once per second   
    if (++waitCount>5) {
      waitCount=0;
      i = (maxJifs-getTime())/jifsPerSecond; //((maxJifs-getTime())/jifsPerSecond);
      if (i<=15 && i != state.moveTime) {
        state.moveTime = i;
        tempBuffer[0]=' ';
        itoa(i, tempBuffer+1, 10);
        drawTextAlt(8-strlen(tempBuffer), HEIGHT-2, tempBuffer);
        drawClock(8,HEIGHT-2);
        soundTick();
      }
    } 

    // Pressed Esc
    switch (inputKey) {
      case KEY_ESCAPE:
      case KEY_ESCAPE_ALT:
        showInGameMenuScreen();
        prevCursorX=(cursorX+1)%6;
        return;
    }

    // Read input for next iteration
    readCommonInput();
  }
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


void clearGameState() {
  // Reset some variables
  wasViewing = 255;
  waitCount = -1;
  requestedMove=NULL;
  forceRender();
}


// Invalidate state variables that will trigger re-rendering of screen items
void forceRender() {
  prevRound = 100;
  prevPlayerCount = 0;
  state.prevActivePlayer = -1;
  forceReadyUpdates = true;
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
    curx = strlen((const char *)buffer);
    drawTextAlt(x,y, buffer);
    drawTextCursor(x+curx,y);
    enableKeySounds();
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

    if (done==1) 
      disableKeySounds();

    return done==1;
  }

  return false;
  
}
