#ifdef _CMOC_VERSION_

// Screen specific player/bet coordinates
const unsigned char playerXMaster[] = { 11,0, 0, 0, 12, 30,30, 30 };
const unsigned char playerYMaster[] = { 18, 18,10,2, 2, 2,10,18 };

const char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
const char playerBetXMaster[] = { 3,9,9,9,2,-7,-7,-7 };
const char playerBetYMaster[] = { -4, -4, 0,4,4,4,0,-4 };

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4
const char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0,
// 5                6                 7                8
   0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};

#endif
