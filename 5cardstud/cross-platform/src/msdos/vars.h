#ifdef __WATCOMC__

#ifndef KEYMAP_H
#define KEYMAP_H

// Screen dimensions for platform

#define WIDTH 40
#define HEIGHT 25
#define QUERY_SUFFIX "" // No extra params for msdos 

/**
 * Platform specific key map for common input
 */

#define KEY_LEFT_ARROW      0x4B00
#define KEY_LEFT_ARROW_2    0x4B01
#define KEY_LEFT_ARROW_3    0x4B02

#define KEY_RIGHT_ARROW     0x4D00
#define KEY_RIGHT_ARROW_2   0x4D01
#define KEY_RIGHT_ARROW_3   0x4D02

#define KEY_UP_ARROW        0x4800
#define KEY_UP_ARROW_2      0x4801
#define KEY_UP_ARROW_3      0x4802

#define KEY_DOWN_ARROW      0x5000
#define KEY_DOWN_ARROW_2    0x5001
#define KEY_DOWN_ARROW_3    0x5002

#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x1B
#define KEY_ESCAPE_ALT   0x011c
#define KEY_SPACE        0x20
#define KEY_BACKSPACE    0x08

#define JOY_UP(v)	(0)
#define JOY_DOWN(v)	(0)
#define JOY_LEFT(v)	(0)
#define JOY_RIGHT(v)	(0)
#define JOY_BTN_1(v)	(0)

/*
  Mapping for converting incoming ALT letters to a standard case
*/

#define LINE_ENDING 0x0A
#define ALT_LETTER_START 65
#define ALT_LETTER_END 90
#define ALT_LETTER_AND 32


#endif /* KEYMAP_H */

int cgetc(void);

#endif /* __WATCOMC__ */

