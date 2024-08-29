#ifdef _CMOC_VERSION_

/*
  Graphics functionality
*/

#include "coco_bool.h"

#include "hires.h"
#include "text.h"
//#include<peekpoke.h>
//#include<string.h>
#include "../platform-specific/graphics.h"
#include "../misc.h"

#define BOTTOM 175
#define RED_VAL_1 ROP_OR(0b11010100)
#define RED_VAL_2 ROP_OR(0b10101010)

//char blah = 'c';
//bool always_render_full_cards = 1;

//unsigned char colorMode=0;

void *screenBuffer;

void resetGraphics2() {
 
}



void
hires_Init()
{
  screenBuffer = (void *) (((word) * (byte *) 0x00BC) << 8);
//  screenBuffer = (void *) (((word) * (byte *) 0x000E) << 8);
#if 0
  openCardGame(screenBuffer, TRUE);
#endif
}


#endif

