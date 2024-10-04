#ifdef __C64__

#ifndef VARS_H
#define VARS_H


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

#define KEY_ESCAPE       CH_ESC // 0x1B
#define KEY_ESCAPE_ALT   1

#define KEY_SPACEBAR     0x20 
#define KEY_BACKSPACE    CH_DEL

#undef KEY_RETURN
#define KEY_RETURN       CH_ENTER // 0x0D

#define CHAR_CURSOR      0xA0

#endif /* VARS_H */

#endif /* __C64__ */