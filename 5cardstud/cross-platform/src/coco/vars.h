#ifdef _CMOC_VERSION_

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 32
#define HEIGHT 24
#define HIRES_Y 1
#define POT_Y_MODIFIER -1

#define QUERY_SUFFIX "&be=1" // Big Endian response for CoCo (68000)

/**
 * Platform specific key map for common input
 */


#define KEY_LEFT_ARROW      0x08
#define KEY_LEFT_ARROW_2    0xF1
#define KEY_LEFT_ARROW_3    0xF2 // ,

#define KEY_RIGHT_ARROW     0x09
#define KEY_RIGHT_ARROW_2   0xF3
#define KEY_RIGHT_ARROW_3   0xF4 // .

#define KEY_UP_ARROW        0x5E
#define KEY_UP_ARROW_2      0x85
#define KEY_UP_ARROW_3      0x86 // -

#define KEY_DOWN_ARROW      0x0A
#define KEY_DOWN_ARROW_2    0x87
#define KEY_DOWN_ARROW_3    0x89 // =

#define KEY_RETURN       0x0D

#define KEY_ESCAPE       0x03
#define KEY_ESCAPE_ALT   0x1B

#define KEY_SPACE        0x20
#define KEY_BACKSPACE    0x08

#define CHAR_CURSOR      0x9F

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
extern const unsigned char playerXMaster[];
extern const unsigned char playerYMaster[];

extern const char playerDirMaster[];
extern const char playerBetXMaster[];
extern const char playerBetYMaster[];

// Simple hard coded arrangment of players around the table based on player count.
// These refer to index positions in the Master arrays above
// Downside is new players will cause existing player positions to move.

extern const char playerCountIndex[];



#endif /* KEYMAP_H */

#endif /* _CMOC_VERSION_ */
