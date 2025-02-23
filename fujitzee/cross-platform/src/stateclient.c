#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#endif /* CMOC_VERSION */
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))
#include "stateclient.h"
#include "misc.h"
#include "fujinet-network.h"

char rx_buf[2048];     // buffer for payload

// Internal to this file
static char url[160];
static char hash[32] = ""; 
static uint16_t rx_pos=0;

void updateState(bool isTables) {
  static char *line, *nextLine, *end, *key, *value, *parent, *arrayPart;
  static bool isKey, inArray;
  static char c;
  static unsigned int i;

  //write_appkey(0x9999,  1, 1, "US");
  // Reset state and vars
  isKey=true; inArray=false;
  playerCount=tableCount=0;
  
  parent = NULL;

  // Load state by looping through result and extracting each string at each EOL character
  end = rx_buf + rx_len;
  
  // debugging
  // POKEW(0x9000, rx_buf);
  // POKEW(0x9004, rx_len);
  // POKEW(0x9008, end);

  // Ensure buffer ends with string terminator
  rx_buf[rx_len]=0;

  line = rx_buf;

  while (line < end) {
    // Capture next line position, in case the current line is shortened 
    // in process of reading
    nextLine=line+strlen(line)+1;
    if (isKey) {
      key = line;

      // Special case - "pl" (players) keys are arrays of key/value pairs
      if (strcmp(key,"pl")==0 || strcmp(key,"null")==0) {

        // If the key is a NULL object, we effectively break out of the array by setting parent to empty
        if (strcmp(key,"null")==0)
          key="";
        
        parent=key;
 
        // Reset isKey since the next line will be a key
        isKey = false;
      } 
    } else {
      value = line;
      if (value[0]==0)
        value = "";

   
      // Set our state variables based on the key
      if (isTables) {
        switch (key[0]) {
          case 't': 
            state.tables[tableCount].table = value;
            break;
          case 'n': 
            state.tables[tableCount].name = value; 
            break;
          case 'p': 
            strcpy(state.tables[tableCount].players, value); 
            break;
          case 'm': 
            strcat(state.tables[tableCount].players, " / ");
            strcat(state.tables[tableCount].players, value); 
            tableCount++;
            break;
          default:
           break;
        }
      } else if (parent[0]=='p') { //(parent,"players")==0) {
        switch (key[0]) {
          case 'n': 
            // Cap name at 8 chars max
            if (strlen(value)>8) 
              value[8]=0; 
            state.players[playerCount].name=value;
            break;
          case 'a':
            state.players[playerCount].alias = value;
            break;
          case 's':
            arrayPart = strtok(value, ",");
            for(i = 0; arrayPart != NULL && i<16; i++) {
                state.players[playerCount].scores[i] = atoi(arrayPart);
                arrayPart = strtok(NULL, ",");
            }

            // Scores is the last property, so increase the player counter
            playerCount++;
            forceReadyUpdates=true;
            break;
          default:
            parent="";
        }
      } else {
        switch (key[0]) {
          case 'h':
            strcpy(hash, value);
            break;
          case 'p':
            //if (strcmp(value, state.prompt)!=0)
              promptChanged = true;
            state.prompt = value;
            break;
          case 'r' :
            state.round = atoi(value);
            break;
          case 'l':
            state.rollsLeft = atoi(value);
            break;
          case 'a':
            state.activePlayer = atoi(value);
            break;
           case 'm':
            state.moveTime = atoi(value);
            break;
          case 'v': 
            state.viewing = value[0]=='1';
            break;
          case 'd':
            state.dice = value;
            break;
          case 'k':
            state.keepRoll = value;
            break;
          case 'c':
            arrayPart = strtok(value, ",");
            for(i = 0; arrayPart != NULL && i<15; i++) {
                state.validScores[i] = atoi(arrayPart);
                arrayPart = strtok(NULL,  ",");
            }
            break;
        } 
      }
      
    }
  
    isKey = !isKey;
    line=nextLine;
  }  
 // sprintf(tempBuffer, "%i", playerCount);
 // write_appkey(0x9999,  0x44, 0x44, tempBuffer);

 
}

/*
 * @brief Makes an Api call, returning true if valid payload received
 * Returns API_CALL_*:
 *  1 - successfully received a payload
 *  2 - async - received payload, still in process, call for more data
 *  0 - error - aborted
*/
uint8_t apiCall(char *path, bool isAsync) {
  static int16_t n;
  static uint8_t* buf;
  
  // If a synchronous call, or starting a new async call,initialize things
  if (rx_pos == 0 || !isAsync) {
    
    // First check if we were in the middle of an async call but we are now requesting sync
    // If so, abort the earlier async call
    if (!isAsync && rx_pos>0) 
      network_close(url);

    // Setup the url
    strcpy(url, serverEndpoint);
    strcat(url, path);
    strcat(url, query);
    strcat(url, strlen(query)==0 ? "?" : "&");
    strcat(url, "raw=1&lc=1&hash=");
    strcat(url, hash);
    
    // Initialize start of buffer for async calls, and reset position
    buf = rx_buf;
    rx_pos = 0;
  }

  // Network error. Reset position and abort.
  if (network_open(url, OPEN_MODE_HTTP_GET_H, OPEN_TRANS_NONE)) {
    rx_pos=0;
    return API_CALL_ERROR;
  }

  if (isAsync) {
    n = network_read_nb(url, buf, 40);

    // If not finished, increment the buffer/position and return 2
    if (fn_network_error != 136) {  
      if (n>0) {
        rx_pos+=n;
        buf+=n;
      }
      return API_CALL_PENDING;
    }
  } else { 
    // Synchronous call
    rx_pos = network_read(url, rx_buf, sizeof(rx_buf));
  }

  // Request is complete
  network_close(url);

  rx_len=rx_pos;

  // Reset position (indicates async request no longer in process)
  rx_pos=0;
   
  return API_CALL_SUCCESS;
}

void sendMove(char* move) {
  skipApiCall=0;
  requestedMove = move;
}

uint8_t getStateFromServer()
{
  static uint8_t apiCallResult;

  // Since we are making an asynchronous call, only update the path if there
  // isn't already a request in progress
  if (rx_pos == 0) {
    if (requestedMove) {  

      // Copy requested move to the temp buffer it is not already one and the same
      if (requestedMove != tempBuffer)  
        strcpy(tempBuffer, requestedMove);

      // Clear hash when sending a move
      strcpy(hash, "");
      requestedMove=NULL;
    } else {
      strcpy(tempBuffer, "state");
    }
  }
  
  // Send an async request - if a request is currently in process, it will resume, ignoring the passed string 
  if ((apiCallResult = apiCall(tempBuffer, false)) == API_CALL_SUCCESS) {
    
    // If the request finished and at least 3 character retrieved, update the state, otherwise assume it is a "no change" response
    if (rx_len>2)
      updateState(false);
    else
      return STATE_UPDATE_NOCHANGE;
    
  }

  return apiCallResult;
}
