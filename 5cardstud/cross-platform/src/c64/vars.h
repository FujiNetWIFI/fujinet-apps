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
#define KEY_BACKSPACE    0x14

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 0x61
#define ALT_LETTER_END 0x7A
#define ALT_LETTER_AND -32 //0x5f

#define QUERY_SUFFIX ""

#endif /* KEYMAP_H */

#endif /* __C64__ */

