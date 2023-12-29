#include <string.h>

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))

char rx_buf[1024];     // buffer for json payload

void updateState() {
  static char *line, *nextLine, *end, *key, *value, *parent;
  static bool isKey, inArray;
  static char c;

  // Reset state and vars
  isKey=true; inArray=false;
  playerCount=validMoveCount=tableCount=0;
  
  // Load state by looping through result and extracting each string at each EOL character
  end = rx_buf + rx_len;

  // Replace line endings with null terminator
  POKE(end+1,0);
  
  // Normalize state receive buffer in preparation for parsing
  for(line=rx_buf;line<end;++line) {
    c=PEEK(line);

    // Convert line endings to string terminators 
    if (c==LINE_ENDING)
      POKE(line,0);
    
    // Convert all letters to match CC65 platform specific lowercase mapping for key name comparisons
    else if (c>=ALT_LETTER_START && c<=ALT_LETTER_END) 
      POKE(line,c & ALT_LETTER_AND);
    
  }

  line = rx_buf;

  while (line < end) {
    // Capture next line position, in case the current line is shortened 
    // in process of reading
    nextLine=line+strlen(line)+1;
    if (isKey) {
      key = line;

      // Special case - "players" and "validMoves" keys are arrays of key/value pairs
      if (strcmp(key,"players")==0 || strcmp(key,"validmoves")==0 || strcmp(key,"null")==0) {

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
      if (strlen(key)==1) {
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
        }
      } else if (strcmp(key,"lastresult")==0)
        state.lastResult = value;
      else if (strcmp(key,"round")==0)
        state.round = atoi(value);
      else if (strcmp(key,"pot")==0)
        state.pot = atoi(value);
      else if (strcmp(key,"activeplayer"  )==0)
        state.activePlayer = atoi(value);
      else if (strcmp(key,"viewing")==0)
        state.viewing = value=="1";
      else if (strcmp(key,"movetime")==0) {
        state.moveTime = atoi(value);
        //timer // Reset timer when we get an updated movetime
      } else if (strcmp(parent,"validmoves")==0) { 
        if (strcmp(key,"move")==0)
          state.validMoves[validMoveCount].move = value;
        else if (strcmp(key,"name")==0) {
          state.validMoves[validMoveCount].name = value;
          validMoveCount++;
        } else {
          parent="";
        }
      } else if (strcmp(parent,"players")==0) {
        if (strcmp(key,"name")==0) {
          // Cap name at 8 chars max
          if (strlen(value)>8) 
            value[8]=0; 
          state.players[playerCount].name=value;
        } else if (strcmp(key,"status")==0)
          state.players[playerCount].status = atoi(value);
        else if (strcmp(key,"bet")==0)
          state.players[playerCount].bet = atoi(value);
        else if (strcmp(key,"move")==0)
          state.players[playerCount].move = value;
        else if (strcmp(key,"purse")==0)
          state.players[playerCount].purse = atoi(value);
        else if (strcmp(key,"hand")==0) {
          state.players[playerCount].hand = value;
          playerCount++;
        } else {
          parent="";
        }
      }
        
    }
  
    isKey = !isKey;
    line=nextLine;
  }  
}

bool apiCall(const char *path) {
  strcpy(urlBuffer, serverEndpoint);
  strcat(urlBuffer, path);
  strcat(urlBuffer, query);
 // drawStatusText(urlBuffer);
  //drawBuffer();
  //cgetc();
  if ((rx_len = getJsonResponse(urlBuffer, rx_buf, sizeof(rx_buf)))<1)
    return false;

  return true;
}

bool getStateFromServer()
{
  if (requestedMove) {    
    strcpy(tempBuffer, "move/");
    strcat(tempBuffer, requestedMove);
    requestedMove=NULL;
  } else {
    strcpy(tempBuffer, "state");
  }
  
  if (apiCall(tempBuffer)) {
    updateState();
    return true;
  }
  return false;
}
