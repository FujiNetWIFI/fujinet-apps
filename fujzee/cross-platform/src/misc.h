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
#include <stdbool.h>
#include <stdint.h>

// FujiNet AppKey settings. These should not be changed
#define AK_LOBBY_CREATOR_ID 1     // FUJINET Lobby
#define AK_LOBBY_APP_ID 1         // Lobby Enabled Game
#define AK_LOBBY_KEY_USERNAME 0   // Lobby Username key
#define AK_LOBBY_KEY_SERVER 3     // Fujzee registered as Lobby appkey 3 at https://github.com/FujiNetWIFI/fujinet-firmware/wiki/SIO-Command-$DC-Open-App-Key

// Fu
#define AK_CREATOR_ID 0xE41C      // Eric Carr//s creator id
#define AK_APP_ID 3               // Fujzee App ID
#define AK_KEY_PREFS 0            // Preferences

#define PREF_HELP 0  // 1/2 seen help screen no/yes
#define PREF_COLOR 1 // 1/2 - Color mode mono/color

#define PLAYER_MAX 6

typedef struct {
  char* table;
  char* name;
  char players[6];
} Table;

typedef struct {
  char* name;
  char alias;
  int16_t scores[16];
} Player;

typedef struct {
  char* move;
  char* name; 
} ValidMove;

typedef struct {
  char* prompt;
  uint8_t round;
  uint8_t rollsLeft;
  int8_t activePlayer;
  uint8_t moveTime;
  uint8_t viewing;
  char* dice;
  char* kept;
  Player players[PLAYER_MAX];
  int16_t validScores[15];

  Table tables[10];
} GameState;


extern unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
extern signed char inputDirX, inputDirY;
extern uint16_t prevPot, rx_len, maxJifs;
extern bool noAnim, doAnim, finalFlip, inputTrigger;
extern char tempBuffer[128];
extern char query[50];
extern char playerName[12];
extern char serverEndpoint[50];

extern GameState state;

// Common local scope temp variables
extern unsigned char h, i, j, k, x, y, xx;
extern unsigned char playerX[PLAYER_MAX], playerY[PLAYER_MAX], moveLoc[5];
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

void read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint8_t* destination);
void write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint8_t *inputString);

#endif /* MISC_H */