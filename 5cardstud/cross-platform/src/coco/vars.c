#ifdef _CMOC_VERSION_

#include "vars.h"

// Screen specific player/bet coordinates
#ifndef COCO3 
unsigned char playerXMaster[] = { 11,0, 0, 0, 11, 30,30, 30 };
unsigned char playerYMaster[] = { 18, 18,10,2, 2, 2,10,18 };

char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
char playerBetXMaster[] = { 3,10,10,10,3,-8,-8,-8 };
char playerBetYMaster[] = { -4, -4, 0,4,4,4,0,-4 };

#else /* CoCo 3*/
unsigned char playerXMaster[] = { 17,1, 1, 1, 16, 37,37, 37 };
unsigned char playerYMaster[] = { 18, 18,10,2, 2, 2,10,18 };

char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
char playerBetXMaster[] = { 1,10,10,10,3,-8,-8,-8 };
char playerBetYMaster[] = { -2, -2, 1,5,5,5,1,-2 };

#endif


// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4
char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0,
// 5                6                 7                8
   0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};

#endif
