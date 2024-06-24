/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef SPRITE_H
#define SPRITE_H

unsigned char icon_get(char *c);

typedef struct
{
    unsigned char y;
    unsigned char x;
    unsigned char sprite_pattern;
    unsigned char color_code : 4;
    unsigned char reserved : 3;    // always zero
    unsigned char early_clock : 1; // 1=shift left horizontally 32 pixels

} SPRITE_ATTRIBUTE;

extern const unsigned char sprite_data[32 * 8];
extern SPRITE_ATTRIBUTE sprite_attributes[32];

/* MEMORY MAP

   VRAM_GENERATOR_TABLE
   0000 - 07FF - char set

   VRAM_COLOR_TABLE
   2000 - 07FF - color set

   VRAM_SPRITE_GENERATOR_TABLE
   3800 - 38FF - sprite table

   VRAM_SPRITE_ATTRIBUTES
   3D00 - 3D7F - sprite attributes
*/

#define VRAM_SPRITE_GENERATOR_TABLE 0x3800 // Library of all sprite patterns (Length 256)
#define VRAM_SPRITE_ATTRIBUTES      0x1B00 // (Length 128)


unsigned char get_sprite(char *c);
void clear_all_sprites();
void display_sprites(void);
void write_sprites(void);
void save_sprite(unsigned char x, unsigned char y, unsigned char icon, bool day);


#endif /* ICON_H */
