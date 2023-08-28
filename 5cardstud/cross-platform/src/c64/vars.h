#ifdef __C64__

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 40
#define HEIGHT 25


/**
 * Platform specific key map for common input
 */

#define KEY_LEFT_ARROW      0x08
#define KEY_LEFT_ARROW_2    0x9D
#define KEY_LEFT_ARROW_3    0x2C // ,

#define KEY_RIGHT_ARROW     0x15
#define KEY_RIGHT_ARROW_2   0x1D
#define KEY_RIGHT_ARROW_3   0x2E // .

#define KEY_UP_ARROW        0x0B
#define KEY_UP_ARROW_2      0x91
#define KEY_UP_ARROW_3      0x2D // -

#define KEY_DOWN_ARROW      0x0A
#define KEY_DOWN_ARROW_2    0x11
#define KEY_DOWN_ARROW_3    0x3D // =

#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x5F
#define KEY_ESCAPE_ALT   0x03
#define KEY_SPACE        0x20

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 0x61
#define ALT_LETTER_END 0x7A
#define ALT_LETTER_AND 0x5f

/*
 Screen related variables
*/


// Screen specific player/bet coordinates
const unsigned char playerXMaster[] = { 17,1, 1, 1, 15, 37,37, 37 };
const unsigned char playerYMaster[] = { 19, 18, 11, 3, 2,3,11,18 };

const char playerDirMaster[] = { 1,1,1,1,1,-1,-1,-1 };
const char playerBetXMaster[] = { 1,10,10,10,3,-8,-8,-8 };
const char playerBetYMaster[] = { -3, -2, 1,4,5,4,1,-2 };

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4  
const char playerCountIndex[] = {0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0, 
// 5                6                 7                8
   0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7};



#endif /* KEYMAP_H */

#endif /* __C64__ */