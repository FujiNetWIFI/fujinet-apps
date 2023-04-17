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


extern unsigned char sprite_set[];
extern unsigned char sprite_color[];

#endif
