/**
 * @brief   5-card-stud
 * @author  Eric Carr, Thomas Cherryhomes, (insert names here)
 * @license gpl v. 3
 * @verbose main
 */

#ifdef _CMOC_VERSION_
#include "coco/coco_bool.h"
#include <fujinet-network.h>
#else
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#endif /* _CMOC_VERSION_ */

#include "platform-specific/graphics.h"
#include "platform-specific/util.h"
#include "platform-specific/input.h"
#include "misc.h"
#include "platform-specific/network.h"
#include "platform-specific/appkey.h"
#include "platform-specific/sound.h"

#include "stateclient.h"
#include "gamelogic.h"
#include "screens.h"

// Store default server endpoint in case lobby did not set app key
char serverEndpoint[50] = "N:https://5card.carr-designs.com/";
//char serverEndpoint[50] = "N:http://192.168.247.3/";
//char serverEndpoint[50] = "http://127.0.0.1:8080/"; // "N: for apple, but not C64"

char query[50] = ""; //?table=blue&player=ERICAPL2";
char playerName[12] = "";

GameState state;

// State helper vars
unsigned char playerCount, prevPlayerCount, validMoveCount, prevRound, tableCount, currentCard, cardIndex, xOffset, fullFirst, cursorX, cursorY, waitCount, inputKey, wasViewing;
signed char inputDirX, inputDirY;
uint16_t prevPot, rx_len, maxJifs;
bool noAnim, doAnim, finalFlip, inputTrigger;

unsigned char playerX[8], playerY[8], moveLoc[5];
signed char playerBetX[8], playerBetY[8], playerDir[8];

// Common local scope temp variables
unsigned char h, i, j, k, x, y, xx;
char tempBuffer[128];

char prefs[4];

char *hand, *requestedMove;

#ifdef _CMOC_CHECK_STACK_OVERFLOW_
void stackOverflowHandler(void *addressOfFailedCheck, void *stackRegister)
{
    printf("[FAIL: %p, %p]\n", addressOfFailedCheck, stackRegister);
    exit(1);
}
#endif

char *http_url = "n:http://api.open-notify.org/astros.json";
char *https_url = "n:https://oldbytes.space/api/v1/timelines/public?limit=1";

uint8_t buf[384];
  static uint16_t count, bw;
  int n;
  uint8_t c;
static uint8_t err = 0;

#if 0
// test code only
void http_no_parse(void)
{
    unsigned char retries=5;

    printf("\nHTTP NO PARSE, 5 ATTEMPTS.\n");

    while(retries--)
    {
printf("network open:\n");
        network_open(serverEndpoint,12,1);
printf("Enter to continue\n");
readline();

printf("network status:\n");
        network_status(serverEndpoint,&bw,&c,&err);
printf("Enter to continue\n");
readline();

        if(bw>sizeof(buf)) bw= sizeof(buf);

printf("network read:\n");
        network_read(serverEndpoint,buf,bw);
        buf[sizeof(buf)-1]= 0;
printf("Enter to continue\n");
readline();


printf("network close:\n");
        network_close(serverEndpoint);
        printf("#%u - BW: %u BYTES. PRESS ENTER.", retries, bw);
        printf("%s\n", buf);
readline();
        //getchar();
    }
}
#endif

#ifdef _CMOC_VERSION_
int main(void)
#else
void main(void)
#endif /* _CMOC_VERSION_ */
{ 
  int i;

#ifdef _CMOC_CHECK_STACK_OVERFLOW_
  set_stack_overflow_handler(stackOverflowHandler);
#endif

#if 0
printf("network test: \n");
sleep(1);

printf("network_init: \n");
  network_init();
for (i=0; i<3; i++)
{
printf("apiCall: \n");
  //apiCall("tables");
  sleep(1);
}
printf("http_no_parse: \n");
http_no_parse();

return 0;
#endif

#if 1
printf("initGraphics\n");
  initGraphics(); 
//  initSound();
//sleep(10);
//closeCardGame();
//return 0;
printf("test version\n");

#ifdef _CMOC_VERSION_
//  network_init();
#endif

#if 0
  apiCall("tables");
  return 0;
#endif 

printf("loadPrefs: \n");
  loadPrefs();

printf("showWelcomScreen: \n");
  showWelcomScreen();
printf("showTableSelectionScreen: \n");
  showTableSelectionScreen();

#if 0
  // Main in-game loop
  while (true) {

    // Get latest state and draw on screen, then prompt player for move if their turn
    if (getStateFromServer()) {
      showGameScreen();
      requestPlayerMove();
    } else {
       // Wait a bit to avoid hammering the server if getting bad responses
       pause(30);
    }
    
    // Handle other key presses
    readCommonInput();
    
    switch(inputKey) {
      case KEY_ESCAPE: // Esc
      case KEY_ESCAPE_ALT: // Esc Alt
        showInGameMenuScreen();  
        break;
    }
    
  }
  #endif
  #endif 

#ifdef _CMOC_VERSION_
  closeCardGame();
  return 0;
#endif /* CMOC_VERSION_  */
}
