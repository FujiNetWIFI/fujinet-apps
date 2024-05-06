#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <string.h>
#endif /* CMOC_VERSION */
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))
#include<stdio.h>

char rx_buf[512];     // buffer for json payload

void updateState() {
  static char *line, *nextLine, *end, *key, *value, *parent;
  static bool isKey, inArray;
  static char c;
  unsigned int lineNum=0;

  //write_appkey(0x9999,  1, 1, "US");
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
    
    // Convert all letters to certain case if needed (e.g. C64)
    else if (c>=ALT_LETTER_START && c<=ALT_LETTER_END) 
      POKE(line,c + ALT_LETTER_AND);
    
  }
  //write_appkey(0x9999,  1, 2, rx_buf);

  line = rx_buf;

  while (line < end) {
    // Capture next line position, in case the current line is shortened 
    // in process of reading
    nextLine=line+strlen(line)+1;
    lineNum++;
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

     // sprintf(tempBuffer, "%i:%s (%i) %s=%s, name=%s", lineNum, parent, playerCount, key, value, state.players[playerCount].name);
     // write_appkey(0x9999,  0x44, 0x44, tempBuffer);

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
           // strcat(state.tables[tableCount].players, " / ");
           // strcat(state.tables[tableCount].players, value); 
            tableCount++;
            break;
        }
      } else if (parent[0]== 'v') { //strcmp(parent,"validmoves")==0) { 
        switch (key[0]) {
          case 'm': // (if (strcmp(key,"move")==0) {
            state.validMoves[validMoveCount].move = value;
            break;
          case 'n': //"name")==0) {
            state.validMoves[validMoveCount].name = value;
            validMoveCount++;
            break;
          default:
            parent="";
        }
      } else if (parent[0]=='p') { //(parent,"players")==0) {
        switch (key[0]) {
          case 'n': //if (strcmp(key,"name")==0) {
            // Cap name at 8 chars max
            if (strlen(value)>8) 
              value[8]=0; 
            state.players[playerCount].name=value;
            break;
          case 's': //"status")==0) {
           // state.players[playerCount].status = atoi(value);
            break;
          case 'b': //"bet")==0) {
            //state.players[playerCount].bet = atoi(value);
            break;
          case 'm'://"move")==0) {
            state.players[playerCount].move = value;
            break;
          case 'p'://"purse")==0) {
            state.players[playerCount].purse = atoi(value);
            break;
          case 'h': //"hand")==0) {
            state.players[playerCount].hand = value;
            // Hand is the last property, so increase the player counter
            playerCount++;
            break;
          default:
            parent="";
        }
      } else {
       
        switch (key[0]) {
          case 'l': //"lastresult")==0) { 
            state.lastResult = value;
            break;
          case 'r' : //"round")==0) {
            state.round = atoi(value);
            break;
          case 'p': //"pot")==0) {
            state.pot = atoi(value);
            break;
          case 'a': //"activeplayer"  )==0) {
            state.activePlayer = atoi(value);
            break;
          case 'v': //"viewing")==0) {
            state.viewing = value[0]=='1';//strcmp(value, "1")==0;
            break;
          case 'm': //"movetime")==0) {
            state.moveTime = atoi(value);
            break;
          //timer // Reset timer when we get an updated movetime
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
  //drawStatusText(urlBuffer);
  //drawBuffer();
  //cgetc();
  //pause(10);
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
