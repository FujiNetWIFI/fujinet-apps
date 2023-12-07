#ifdef __ATARI__

#ifndef KEYMAP_H
#define KEYMAP_H
/**
 * Cross platform key map for common input
 */

#define KEY_LEFT_ARROW      0x1E
#define KEY_LEFT_ARROW_2    0x2B // +
#define KEY_LEFT_ARROW_3    0x3C // <

#define KEY_RIGHT_ARROW     0x1f
#define KEY_RIGHT_ARROW_2   0x2A // *
#define KEY_RIGHT_ARROW_3   0x3E // >

#define KEY_UP_ARROW        0x1C
#define KEY_UP_ARROW_2      0x2D // -
#define KEY_UP_ARROW_3      0xFF // unused

#define KEY_DOWN_ARROW      0x1D
#define KEY_DOWN_ARROW_2    0x3D // =
#define KEY_DOWN_ARROW_3    0xFE // unused

#define KEY_FIRE_RETURN     0x9B
#define KEY_FIRE_SPACE      0x20
#define KEY_FIRE_ESCAPE     0x1B


#endif /* KEYMAP_H */

#endif /* __ATARI__ */