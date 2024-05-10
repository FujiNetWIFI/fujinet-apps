#ifndef MISC_H
#define MISC_H
/* 
 * Include CC65 style Joystick defines for Adam - There is a probably a better way to do this.
 */

#ifdef _CMOC_VERSION_
#include "coco/joystick.h"
#else
#ifdef __ADAM__
#include "adam/joystick.h"
#else
#include <joystick.h>
#include <conio.h>
#endif /* __ADAM__ */
#endif /* _CMOC_VERSION_ */
#include "platform-specific/graphics.h"
#include "platform-specific/input.h"

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

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  char * table;
  char * name;
  char players[8];
} Table;

typedef struct {
  char * name;
  unsigned char status;
  uint16_t bet;
  char * move;
  uint16_t purse;      
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


extern unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
extern signed char inputDirX, inputDirY;
extern int prevPot, rx_len, maxJifs;
extern bool noAnim, doAnim, finalFlip, inputTrigger;
extern char tempBuffer[128];
extern char query[50];
extern char playerName[12];
extern char serverEndpoint[50];

extern GameState state;

// Common local scope temp variables
extern unsigned char h, i, j, k, x, y, xx;
extern unsigned char playerX[8], playerY[8], moveLoc[5];
extern signed char playerBetX[8], playerBetY[8], playerDir[8];
extern char *hand, *requestedMove;


// Screen specific player/bet coordinates
extern const unsigned char playerXMaster[] ;
extern const unsigned char playerYMaster[] ;
extern const char playerDirMaster[] ;
extern const char playerBetXMaster[];
extern const char playerBetYMaster[] ;

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.
extern const char playerCountIndex[] ;



void pause(unsigned char frames);
void cycleColors();
void clearCommonInput();
void readCommonInput();

#endif /* MISC_H */