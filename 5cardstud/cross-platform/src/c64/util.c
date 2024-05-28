#ifdef __C64__

#include<peekpoke.h>
#include<stdlib.h>

void resetGraphics();


/*
 Screen related variables
*/


// Screen specific player/bet coordinates
const unsigned char playerXMaster[] = { 17,1, 1, 1, 15, 37,37, 37 };
const unsigned char playerYMaster[] = { 18, 18, 10, 2, 2,2,10,18 };

const char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
const char playerBetXMaster[] = { 1,10,10,10,3,-8,-8,-8 };
const char playerBetYMaster[] = { -2, -2, 1,5,5,5,1,-2 };

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4  
const char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0, 
// 5                6                 7                8
   0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};



void resetTimer() {
  POKE(162,0);
  POKE(161,0);
}

int getTime() {
  return (PEEK(161)*256)+PEEK(162);
}

void quit() { 
  resetGraphics();
  exit(0);
}

#endif /* __C64__ */