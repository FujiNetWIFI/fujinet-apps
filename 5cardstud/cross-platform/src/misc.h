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
#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdbool.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */
// FujiNet AppKey settings. These should not be changed
#define AK_LOBBY_CREATOR_ID 1     // FUJINET Lobby
#define AK_LOBBY_APP_ID 1         // Lobby Enabled Game
#define AK_LOBBY_KEY_USERNAME 0   // Lobby Username key
#define AK_LOBBY_KEY_SERVER 1     // 5 Card Stud Client registered as Lobby appkey 1

// 5 Card Stud client
#define AK_CREATOR_ID 0xE41C      // Eric Carr//s creator id
#define AK_APP_ID 1               // 5 Card/Poker App ID
#define AK_KEY_PREFS 0            // Preferences

#define PREF_HELP 0  // 1/2 seen help screen no/yes
#define PREF_COLOR 1 // 1/2 - Color mode mono/color
#define PREF_SOUND 2 // 1/2 - Sound Enabled yes/no

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
#ifdef _CMOC_VERSION_
extern unsigned char noAnim, doAnim, finalFlip, inputTrigger;
#else
extern bool noAnim, doAnim, finalFlip, inputTrigger;
#endif /* _CMOC_VERSION_ */
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
extern char prefs[4];


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
void clearCommonInput();
void readCommonInput();
void loadPrefs();
void savePrefs();

#endif /* MISC_H */
