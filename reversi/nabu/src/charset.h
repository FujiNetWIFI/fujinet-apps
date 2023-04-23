#ifndef _NWD_CHARSET_H
#define _NWD_CHARSET_H

#define PatternTable 0x0000  // 2048 The generator table
#define NameTable    0x1800  // 768 bytes The screen
#define ColorTable   0x2000  // 32 bytesColor Table

// 0 = Transparent
// 1 = Black
// 2 = Medium Green
// 3 = Light Green
// 4 = Dark Blue
// 5 = Light Blue
// 6 = Dark Red
// 7 = Cyan
// 8 = Medium Red
// 9 = Light Red
// A = Dark Yellow
// B = Light Yellow
// C = Dark Green
// D = Magenta
// E = Gray
// F = White

// vdp_color(fgcol, bgcol, border)
#define BACKGROUND_COLOUR_TEXT     1, 3, 3     
#define BACKGROUND_COLOUR_GRAPHICS 1, 0xc, 0xc

#define THICK_TOP_LEFT          0
#define THICK_TOP_ACROSS        THICK_TOP_LEFT+1
#define THICK_TOP_RIGHT         THICK_TOP_LEFT+2

#define THICK_DOWN_LEFT         THICK_TOP_LEFT+3

#define THICK_BOTTOM_LEFT       8
#define THICK_BOTTOM_ACROSS     THICK_BOTTOM_ACROSS+1
#define THICK_BOTTOM_RIGHT      THICK_BOTTOM_ACROSS+2

#define THICK_DOWN_RIGHT        THICK_BOTTOM_ACROSS+3

#define DARK_GREEN_BLOCK        16

#define WHITE_LINE_COLOUR       160
#define BLACK_LINE_COLOUR       152

#define STATUS_BLOCK            168

#define THIN_TOP_LEFT           DARK_GREEN_BLOCK+1
#define THIN_TOP_RIGHT          DARK_GREEN_BLOCK+2
#define THIN_TOP_ACROSS         DARK_GREEN_BLOCK+3

#define THIN_BOTTOM_LEFT        DARK_GREEN_BLOCK+4
#define THIN_BOTTOM_RIGHT       DARK_GREEN_BLOCK+5
#define THIN_BOTTOM_ACROSS      DARK_GREEN_BLOCK+6

#define THIN_DOWN_LEFT          DARK_GREEN_BLOCK+7
#define THIN_DOWN_RIGHT         DARK_GREEN_BLOCK+8

#define WHITE_TOP_LEFT          128
#define WHITE_TOP_RIGHT         WHITE_TOP_LEFT+1
#define WHITE_BOTTOM_LEFT       WHITE_TOP_LEFT+2
#define WHITE_BOTTOM_RIGHT      WHITE_TOP_LEFT+3

#define BLACK_TOP_LEFT          136
#define BLACK_TOP_RIGHT         BLACK_TOP_LEFT+1
#define BLACK_BOTTOM_LEFT       BLACK_TOP_LEFT+2
#define BLACK_BOTTOM_RIGHT      BLACK_TOP_LEFT+3


extern unsigned char character_set[];
extern unsigned char character_color[];

void init_character_set();

#endif
