/**
 * Cursor routines
 */

#include <video/tms99x8.h>
#include "cursor.h"

static const unsigned char selector_pattern_right[8] =
  {0x00,0x60,0x78,0x7E,0x7E,0x78,0x60,0x00};

static const unsigned char selector_pattern_left[8] =
  {0x00,0x06,0x1E,0x7E,0x7E,0x1E,0x06,0x00};

static const unsigned char blank[8] =
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static const unsigned char _cursor[8] =
  {0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF};

void cursor(bool t)
{
  vdp_set_sprite_8(0,t ? _cursor : blank);
}

void cursor_pos(unsigned char x, unsigned char y)
{
  vdp_put_sprite_8(0,x<<3,y<<3,0,VDP_INK_DARK_BLUE);
}

void selector(unsigned char t)
{
  switch(t)
    {
    case 0:
      vdp_set_sprite_8(0,blank);
      break;
    case 1:
      vdp_set_sprite_8(0,selector_pattern_right);
      break;
    case 2:
      vdp_set_sprite_8(0,selector_pattern_left);
      break;
    }
}

void selector_pos(unsigned char x, unsigned char y)
{
  vdp_put_sprite_8(0,x<<3,y<<3,0,VDP_INK_DARK_RED);
}
