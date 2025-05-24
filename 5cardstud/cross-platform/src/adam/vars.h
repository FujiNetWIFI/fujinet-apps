#ifdef __ADAM__

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 32
#define HEIGHT 24


/**
 * Platform specific key map for common input
 */

#define KEY_LEFT_ARROW      0xa3
#define KEY_LEFT_ARROW_2    0x9D
#define KEY_LEFT_ARROW_3    0x2C // ,

#define KEY_RIGHT_ARROW     0xa1
#define KEY_RIGHT_ARROW_2   0x1D
#define KEY_RIGHT_ARROW_3   0x2E // .

#define KEY_UP_ARROW        0xa0
#define KEY_UP_ARROW_2      0x91
#define KEY_UP_ARROW_3      0x2D // -

#define KEY_DOWN_ARROW      0xa2
#define KEY_DOWN_ARROW_2    0x11
#define KEY_DOWN_ARROW_3    0x3D // =

#define KEY_RETURN       0x0d
#define KEY_ESCAPE       0x1b
#define KEY_ESCAPE_ALT   0x03
#define KEY_SPACE        0x20
#define KEY_BACKSPACE    0x08

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 0x0
#define ALT_LETTER_END 0x0
#define ALT_LETTER_AND 0x0

/*
 Screen related variables
*/


// Screen specific player/bet coordinates
extern const unsigned char playerXMaster[];
extern const unsigned char playerYMaster[];

extern const char playerDirMaster[];
extern const char playerBetXMaster[];
extern const char playerBetYMaster[];

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

//                               2                3                4
extern const char playerCountIndex[];


#endif /* KEYMAP_H */

#endif /* __ADAM__ */

