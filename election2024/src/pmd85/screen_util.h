#ifndef SCREEN_UTIL_H
#define SCREEN_UTIL_H

#define ACE_BLUE    0x8080
#define ACE_RED     0x4040
#define ACE_MAGENTA 0xC0C0
#define ACE_GREEN   0x0000
#define ACE_CYAN    0x8000
#define ACE_YELLOW  0x4000
#define ACE_WHITE   0xC000

#define MASK_KEEP_BITMAP    0x3F0000
#define MASK_KEEP_COLOR     0xC00000

#define PATTERN_SOLID       0x3F3F
#define PATTERN_BLANK       0x0000
#define PATTERN_CHESSBOARD  0x2A15
#define PATTERN_SPARE       0x1002
#define PATTERN_REVERSE     (MASK_KEEP_BITMAP | PATTERN_SOLID)

extern void __FASTCALL__ screen_fill(int pattern);

extern void __CALLEE__  screen_set_region(char x, char y, char w, char h);
extern void __FASTCALL__ screen_fill_region(long pattmask);

#endif // SCREEN_UTIL_H
