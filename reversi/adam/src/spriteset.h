#ifndef _NWD_SPRITESET_H
#define _NWD_SPRITESET_H

typedef struct
{
    unsigned char y;
    unsigned char x;
    unsigned char sprite_pattern;
    unsigned char color_code : 4;
    unsigned char reserved : 3;    // always zero
    unsigned char early_clock : 1; // 1=shift left horizontally 32 pixels

} SPRITE_ATTRIBUTE;

#define TOTAL_SPRITES 12

#define BOARD_START_X 8
#define BOARD_START_Y 3

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

#define SELECTED_COLOR 0x0B
#define MOVING_COLOR   0x0E

extern SPRITE_ATTRIBUTE sprite_attrib[TOTAL_SPRITES];

#define SprAttrTable 0x1b00  // Sprite Attribute
#define SprPatTable  0x3800  // Sprite Pattern

extern unsigned char sprite_set[];
extern unsigned char sprite_color[];

void init_adam_sprites() ;
void movsprite(int x, int y, int trig);

#endif
