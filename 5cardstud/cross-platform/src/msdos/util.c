#include <dos.h>
#include <i86.h>
#include <stdlib.h>

extern unsigned char oldmode;

// Screen specific player/bet coordinates
const unsigned char playerXMaster[] = { 17,1, 1, 1, 15, 37,37, 37 };
const unsigned char playerYMaster[] = { 18, 17, 10, 3, 2,3,10,17 };

const char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
const char playerBetXMaster[] = { 1,10,10,10,3,-8,-8,-8 };
const char playerBetYMaster[] = { -2, -2, 1,4,5,4,1,-2 };

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4
const char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0,
// 5                6                 7                8
    0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};

// /**
//  * @brief attempt to make a pause for given # of milliseconds,
//  * @param ms Milliseconds to wait, rounded to nearest 16.67ms frame.
//  */
// void pause(int ms)
// {
//     int frames = ms >> 4; // / 16, almost 16.67

//     while (frames--)
//     {
//         // Wait until vblank starts
//         while(!(inp(0x3DA) & 0x08));
//         // Wait until vblank stops
// 	    while(inp(0x3DA) & 0x08);
//     }
// }


unsigned int jiffieTimer;

void resetTimer() {
  jiffieTimer=0;
}

int getTime() {
  jiffieTimer+=3;
  return jiffieTimer;
}

void quit()
{
	union REGS r;

	r.h.ah = 0x00;
	r.h.al = oldmode;
	int86(0x10,&r,&r);
	exit(0);
}
