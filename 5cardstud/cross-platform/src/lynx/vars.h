#ifdef __LYNX__


// Screen dimensions for platform
#define WIDTH 40
#define HEIGHT 17

#define TABLE_TOP 3
#define TABLE_ROW_HEIGHT 1
#define HOW_TO_PLAY_ROW_START 0
#define SKIP_FIRST_BUMP   1

/**
 * Platform specific key map for common input
 * Lynx has hardly any keys, only ESCAPE is really valid
 */
#define USE_PLATFORM_SPECIFIC_INPUT

#define KEY_ESCAPE       'R'
#define KEY_ESCAPE_ALT   '3'

// all these values are bogus
#define KEY_LEFT_ARROW      0xA1
#define KEY_LEFT_ARROW_2    0xA2
#define KEY_LEFT_ARROW_3    0xA3 // ,

#define KEY_RIGHT_ARROW     0xB1
#define KEY_RIGHT_ARROW_2   0xB2
#define KEY_RIGHT_ARROW_3   0xB3 // .

#define KEY_UP_ARROW        0xC1
#define KEY_UP_ARROW_2      0xC2
#define KEY_UP_ARROW_3      0xC3 // -

#define KEY_DOWN_ARROW      0xD1
#define KEY_DOWN_ARROW_2    0xD2
#define KEY_DOWN_ARROW_3    0xD3 // =

#define KEY_RETURN          0xE1
#define KEY_SPACE           0xE2
#define KEY_BACKSPACE       0xE3

#define QUERY_SUFFIX  ""


void initPlatformKeyboardInput();


#endif