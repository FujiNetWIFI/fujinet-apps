/**
 * Weather / sprite.c
 *
 * Based on @bocianu's code
 *
 * @author Norman Davie
 *
 */

#include <stdbool.h>
#include <video/tms99x8.h>
#include <eos.h>
#include <string.h>
#include "sprite.h"
#include "constants.h"

SPRITE_ATTRIBUTE sprite_attributes[32];

unsigned char sprite_counter = 0;
unsigned char sprites[32 * 8]; // Length 256;


#define SPRITE_SUNNY 0
#define SPRITE_HALF_SUNNY 1
#define SPRITE_CLOUD_SUN 2
#define SPRITE_BLACK_GREY_CLOUD 3
#define SPRITE_BLACK_GREY_CLOUD_RAIN 4
#define SPRITE_SUN_CLOUD_RAIN 5
#define SPRITE_CLOUD_LIGHTENING 6
#define SPRITE_SNOW 7
#define SPRITE_MIST 8




const unsigned char spritedata[] = {
    // 0 Sunny
    0x04, 0x44, 0x20, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0xDF, 0x1F, 0x1F, 0x0F, 0x07, 0x20, 0x44, 0x04, 0x20, 0x22, 0x04, 0xE0, 0xF0, 0xF8, 0xF8, 0xF8, 0xFB, 0xF8, 0xF8, 0xF0, 0xE0, 0x04, 0x22, 0x20,

    // 1 Half Sunny
    0x04, 0x44, 0x20, 0x07, 0x0F, 0x9F, 0x5F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x20, 0x22, 0x04, 0xE0, 0xF1, 0xFA, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0x00,

    // 2 Cloud 1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0,

    // 3 Cloud 2
    0x00, 0x00, 0x00, 0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00,

    // 4 Cloud 3
    0x00, 0x00, 0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,

    // 5 Cloud 4
    0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // 6 Mist 1
    0x05, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x20, 0x00, 0x40, 0x00, 0x10, 0x00, 0x0B, 0x00, 0xB4, 0x00, 0x08, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x68, 0x00,

    // 7 Mist 2
    0x00, 0x00, 0x0D, 0x00, 0x1B, 0x00, 0x37, 0x00, 0x2F, 0x00, 0x1B, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xD8, 0x00, 0xEC, 0x00, 0xD8, 0x00, 0xEC, 0x00, 0xD0, 0x00, 0x00, 0x00,

    // 8 Moon
    0x03, 0x0F, 0x1F, 0x3F, 0x3E, 0x7E, 0x7C, 0x7C, 0x7C, 0x7C, 0x7E, 0x3E, 0x3F, 0x1F, 0x0F, 0x03, 0xE0, 0xF0, 0xFC, 0x84, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x84, 0xFC, 0xF0, 0xE0,

    // 9 Cloud 5
    0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // A Cloud 6
    0x03, 0x07, 0x6F, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // B Lightning 1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0F, 0x00, 0x00, 0x01, 0x07, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0x00, 0x00, 0xF0, 0x60, 0xC0, 0x80, 0xC0, 0x80, 0x00,

    // C Lightning 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xC0, 0x80, 0x00, 0xF8, 0x30, 0x60, 0xC0, 0xE0, 0xC0, 0x00,

    // D Rain 1
    0x00, 0x00, 0x00, 0x00, 0x08, 0x0A, 0x02, 0x10, 0x14, 0x05, 0x21, 0x28, 0x0A, 0x42, 0x50, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x84, 0xA0, 0x2A, 0x0A, 0x40, 0x54, 0x14, 0x80, 0xA8, 0x20,

    // E Rain 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x21, 0x28, 0x0A, 0x42, 0x50, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x41, 0x55, 0x14, 0x80, 0xAA, 0x22,

    // F Snow 1
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x0A, 0x04, 0x20, 0x51, 0x20, 0x00, 0x44, 0xE0, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x50, 0x22, 0x05, 0x92, 0x40, 0x90, 0x00, 0x4A, 0xE0, 0x44,

    // 10 Snow 2
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x24, 0x80, 0x15, 0x4A, 0xA4, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x02, 0x00, 0x84, 0x01, 0x12, 0x45, 0xA2, 0x48,

    // 11 Lightning 3
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xC0, 0x80, 0xFC, 0x18, 0x30, 0x60, 0xF0, 0xE0, 0x80
};

// https://openweathermap.org/weather-conditions
 
unsigned char get_sprite(char *c)
{
    if (strcmp(c, "01d") == 0 || strcmp(c, "01n") == 0)
        return ICON_CLEAR_SKY;
    else if (strcmp(c, "02d") == 0 || strcmp(c, "02n") == 0)
        return ICON_FEW_CLOUDS;
    else if (strcmp(c, "03d") == 0 || strcmp(c, "03n") == 0)
        return ICON_SCATTERED_CLOUDS;
    else if (strcmp(c, "04d") == 0 || strcmp(c, "04n") == 0)
        return ICON_BROKEN_CLOUDS;
    else if (strcmp(c, "09d") == 0 || strcmp(c, "09n") == 0)
        return ICON_SHOWER_RAIN;
    else if (strcmp(c, "10d") == 0 || strcmp(c, "10n") == 0)
        return ICON_RAIN;
    else if (strcmp(c, "11d") == 0 || strcmp(c, "11n") == 0)
        return ICON_THUNDERSTORM;
    else if (strcmp(c, "13d") == 0 || strcmp(c, "13n") == 0)
        return ICON_SNOW;
    else if (strcmp(c, "50d") == 0 || strcmp(c, "50n") == 0)
        return ICON_MIST;
}

/*
clear_all_sprites 
- ensures all the sprites are in memory and removes existing defined sprites from the display

*/
void clear_all_sprites() 
{
    eos_write_vdp_register(1, 0xE3);
    vdp_vwrite(spritedata, VRAM_SPRITE_GENERATOR_TABLE, sizeof(spritedata));

    sprite_counter = 0;
    memset(sprite_attributes, 208, 32 * sizeof(SPRITE_ATTRIBUTE));

    display_sprites();
}

/*
Displays all the defined sprites at the appropriate positions
*/
void display_sprites(void) 
{ 
    vdp_vwrite(sprite_attributes, VRAM_SPRITE_ATTRIBUTES, 32 * sizeof(SPRITE_ATTRIBUTE)); 
}


/*
Saves a sprite for use, but does not display the sprite until display_sprites is called
*/

void save_sprite(unsigned char x, unsigned char y, unsigned char icon, bool day)
{

    switch (icon)
    {
    case ICON_CLEAR_SKY: // clear sky
        // Sun with clouds
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = day == true ? 0x00 : 32;
        sprite_attributes[sprite_counter].color_code = 0x0A;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;
        break;
    case ICON_FEW_CLOUDS: // few clouds

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 0x08;
        sprite_attributes[sprite_counter].color_code = 0x0F;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = day == true ? 0x04 : 32;
        sprite_attributes[sprite_counter].color_code = 0x0A;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        break;
    case ICON_SCATTERED_CLOUDS: // Scattered clouds
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 0x10;
        sprite_attributes[sprite_counter].color_code = 0x0F;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        break;
    case ICON_BROKEN_CLOUDS: // broken clouds
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 16;
        sprite_attributes[sprite_counter].color_code = 0x0e;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 20;
        sprite_attributes[sprite_counter].color_code = 0x01;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;


        break;
    case ICON_SHOWER_RAIN: // shower rain
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 56;
        sprite_attributes[sprite_counter].color_code = 0x04;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;


        if (day == true)
        {
            sprite_attributes[sprite_counter].y = y;
            sprite_attributes[sprite_counter].x = x;
            sprite_attributes[sprite_counter].sprite_pattern = 16;
            sprite_attributes[sprite_counter].color_code = 0x0e;
            sprite_attributes[sprite_counter].reserved = 0;
            sprite_attributes[sprite_counter].early_clock = 0;
            sprite_counter++;

            sprite_attributes[sprite_counter].y = y;
            sprite_attributes[sprite_counter].x = x;
            sprite_attributes[sprite_counter].sprite_pattern = 20;
            sprite_attributes[sprite_counter].color_code = 0x01;
            sprite_attributes[sprite_counter].reserved = 0;
            sprite_attributes[sprite_counter].early_clock = 0;
            sprite_counter++;


        }
        else
        {
            sprite_attributes[sprite_counter].y = y;
            sprite_attributes[sprite_counter].x = x;
            sprite_attributes[sprite_counter].sprite_pattern = 08;
            sprite_attributes[sprite_counter].color_code = 0x0f;
            sprite_attributes[sprite_counter].reserved = 0;
            sprite_attributes[sprite_counter].early_clock = 0;
            sprite_counter++;

            sprite_attributes[sprite_counter].y = y;
            sprite_attributes[sprite_counter].x = x;
            sprite_attributes[sprite_counter].sprite_pattern = day == true ? 0x04 : 32;
            sprite_attributes[sprite_counter].color_code = 0x0A;
            sprite_attributes[sprite_counter].reserved = 0;
            sprite_attributes[sprite_counter].early_clock = 0;
            sprite_counter++;

        }
        break;
    case ICON_RAIN: // rain
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 56;
        sprite_attributes[sprite_counter].color_code = 0x04;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 8;
        sprite_attributes[sprite_counter].color_code = 0x0f;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 4;
        sprite_attributes[sprite_counter].color_code = 0x0A;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        break;
    case ICON_THUNDERSTORM: // thunderstorm
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 48;
        sprite_attributes[sprite_counter].color_code = 0x0A;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 16;
        sprite_attributes[sprite_counter].color_code = 0x0e;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 20;
        sprite_attributes[sprite_counter].color_code = 0x01;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 56;
        sprite_attributes[sprite_counter].color_code = 0x04;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        break;

    case ICON_SNOW: // snow
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 60;
        sprite_attributes[sprite_counter].color_code = 0x0e;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 16;
        sprite_attributes[sprite_counter].color_code = 0x0f;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 20;
        sprite_attributes[sprite_counter].color_code = 0x01;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

    case ICON_MIST: // mist
        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 28;
        sprite_attributes[sprite_counter].color_code = 0x0f;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        sprite_attributes[sprite_counter].y = y;
        sprite_attributes[sprite_counter].x = x;
        sprite_attributes[sprite_counter].sprite_pattern = 24;
        sprite_attributes[sprite_counter].color_code = 0x0e;
        sprite_attributes[sprite_counter].reserved = 0;
        sprite_attributes[sprite_counter].early_clock = 0;
        sprite_counter++;

        break;
    }
}

