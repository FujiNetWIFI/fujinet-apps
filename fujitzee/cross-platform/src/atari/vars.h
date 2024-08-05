#ifdef __ATARI__

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 40
#define HEIGHT 26


/**
 * Platform specific key map for common input
 */

#define KEY_LEFT_ARROW      CH_CURS_LEFT
#define KEY_LEFT_ARROW_2    1
#define KEY_LEFT_ARROW_3    2 // ,

#define KEY_RIGHT_ARROW     CH_CURS_RIGHT
#define KEY_RIGHT_ARROW_2   3
#define KEY_RIGHT_ARROW_3   4 // .

#define KEY_UP_ARROW        CH_CURS_UP
#define KEY_UP_ARROW_2      5
#define KEY_UP_ARROW_3      6 // -

#define KEY_DOWN_ARROW      CH_CURS_DOWN
#define KEY_DOWN_ARROW_2    7
#define KEY_DOWN_ARROW_3    8 // =

//#define KEY_RETURN       0x0D
#define KEY_ESCAPE       CH_ESC
#define KEY_ESCAPE_ALT   1

#define KEY_SPACEBAR     0x20 // cc65's atari.h KEY_SPACE is wrong
#define KEY_BACKSPACE    CH_DEL

#undef KEY_RETURN
#define KEY_RETURN       CH_ENTER

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 0x61
#define ALT_LETTER_END 0x7A
#define ALT_LETTER_AND 0x5f


#endif /* KEYMAP_H */

#endif /* __ATARI__ */