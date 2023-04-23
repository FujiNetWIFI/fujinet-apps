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


extern SPRITE_ATTRIBUTE sprite_attrib[TOTAL_SPRITES];

#define SprAttrTable 0x1b00  // Sprite Attribute
#define SprPatTable  0x3800  // Sprite Pattern

extern unsigned char sprite_set[];
extern unsigned char sprite_color[];

void init_adam_sprites() ;
void movsprite(int x, int y, int trig);

#endif
