#ifdef __ATARI__

#ifndef VARS_H
#define VARS_H


/**
 * Platform specific key map for common input
 */

#define KEY_LEFT_ARROW      CH_CURS_LEFT
#define KEY_LEFT_ARROW_2    43 // +
#define KEY_LEFT_ARROW_3    60 // <

#define KEY_RIGHT_ARROW     CH_CURS_RIGHT
#define KEY_RIGHT_ARROW_2   42 // *
#define KEY_RIGHT_ARROW_3   62 // >

#define KEY_UP_ARROW        CH_CURS_UP
#define KEY_UP_ARROW_2      45 // -
#define KEY_UP_ARROW_3      2 // DUMMY

#define KEY_DOWN_ARROW      CH_CURS_DOWN
#define KEY_DOWN_ARROW_2    61 // =
#define KEY_DOWN_ARROW_3    3 // DUMMY


#define KEY_ESCAPE       CH_ESC
#define KEY_ESCAPE_ALT   1

#define KEY_SPACEBAR     0x20 // cc65's atari.h KEY_SPACE is wrong
#define KEY_BACKSPACE    CH_DEL

#undef KEY_RETURN
#define KEY_RETURN       CH_ENTER

#define CHAR_CURSOR      0xA0

#endif /* VARS_H */

#endif /* __ATARI__ */