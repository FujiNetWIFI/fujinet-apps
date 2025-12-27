#ifdef __LYNX__

#include "vars.h"

// Screen specific player/bet coordinates
unsigned char playerXMaster[] = { 15,  0,  0, 0, 15, 38, 38, 38 };
unsigned char playerYMaster[] = { 13, 13,  8, 2,  2,  2,  8, 13 };

char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
char playerBetXMaster[] = {  2, 10, 10, 10, 2, -9, -9, -9 };
char playerBetYMaster[] = { -2, -3,  0,  3, 3,  3,  0, -2 };


// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4
char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0,
                              // 5                6                 7                8
                           0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};

#endif
