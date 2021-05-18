#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include "nsio.h"
#include "faux_json.h"

/* A chunk of memory for the screen
   (bitmapped graphics, text window, and display list),
   as described in `atari.cfg`, and found in `segments.s` */
extern unsigned char scr_mem[];

/* Pointers within `scr_mem` */
unsigned char * dlist;
unsigned char * txt_mem;

/* A chunk of memory for the Player/Missile Graphics */
extern unsigned char pmg_mem[];

/* The world map bitmap */
extern unsigned char map_data[];

/* Set up the screen & PMG */
void setup(void) {
  unsigned char i;

  /* Turn off the screen (ANTIC DMA) */
  OS.sdmctl = 0;

  /* Set our pointers */
  txt_mem = scr_mem + (40 * 80);
  dlist = txt_mem + (40 * 4);

  /* Clear everything */
  bzero(scr_mem, (dlist - scr_mem + 1));
  bzero(pmg_mem, 1024);

  /* Generate a display list... */

  /* Black border at the top */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  /* First line of bitmap graphics, with LMS */
  POKE(dlist + 3, DL_LMS(DL_GRAPHICS7));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  /* 79 more lines of graphics */
  for (i = 0; i < 79; i++) {
    POKE(dlist + 6 + i, DL_GRAPHICS7);
  }

  /* First line of text window, with LMS */
  POKE(dlist + 85, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 86, (unsigned int) txt_mem);
  for (i = 0; i < 3; i++) {
    POKE(dlist + 88 + i, DL_GRAPHICS0);
  }

  POKE(dlist + 91, DL_JVB);
  POKEW(dlist + 92, (unsigned int) dlist);

  /* Config PMGs */

  OS.gprior = PRIOR_P03_PF03;
  GTIA_WRITE.sizep0 = PMG_SIZE_NORMAL;
  GTIA_WRITE.hposp0 = 0;
  OS.pcolr0 = 0x00;
  ANTIC.pmbase = ((unsigned int) pmg_mem) >> 8;

  /* Use our display list & enable the screen */
  OS.sdlst = dlist;
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH | DMACTL_DMA_PLAYERS;
  GTIA_WRITE.gractl = GRACTL_PLAYERS;
}

#define X_CENTER 79
#define Y_CENTER 39

#define SPRITE_HEIGHT 8
#define SPRITE_WIDTH 8 /* Players are 8 pixels wide! */

unsigned char sprite[SPRITE_HEIGHT] = {
  0x20, /* ..X.|.... */
  0x50, /* .X.X|.... */
  0xA4, /* X.X.|.X.. */
  0x58, /* .X.X|X... */
  0x1A, /* ...X|X.X. */
  0x05, /* ....|.X.X */
  0x0A, /* ....|X.X. */
  0x04, /* ....|.X.. */
};

void main(void) {
  int i, lat, lon;
  unsigned char n, x, y, key;

  /* Set up the screen */
  setup();

  /* Display the map (simple animation effect) */
  OS.color4 = 2;//scr_mem[3200];
  OS.color0 = 4;//scr_mem[3201];
  OS.color1 = 6;//scr_mem[3202];
  OS.color2 = 8;//scr_mem[3203];

  for (i = 0; i < 40; i++) {
    memcpy((unsigned char *) scr_mem + (39 - i) * 40, (unsigned char *) map_data + (39 - i) * 40, 40);
    memcpy((unsigned char *) scr_mem + (i + 40) * 40, (unsigned char *) map_data + (i + 40) * 40, 40);
    n = OS.rtclok[2] + 1;
    do { } while (OS.rtclok[2] != n);
  }

  OS.color4 = map_data[3200];
  OS.color0 = map_data[3201];
  OS.color1 = map_data[3202];
  OS.color2 = map_data[3203];


  /* MAIN LOOP */

  do {
    /* Fetch the ISS's current position */
    /* http://api.open-notify.org/iss-now.json */
    strcpy(json,
      "{\"timestamp\": 1621322818, \"iss_position\": {\"longitude\": \"-39.1409\", \"latitude\": \"2.1292\"}, \"message\": \"success\"}"
    );
    faux_parse_json("longitude\": \"", 0);
    lon = atoi(json_part);
    faux_parse_json("latitude\": \"", 0);
    lat = atoi(json_part);

    /* Draw the ISS in its position */
  
    /* Map longitude (-180 -> 180 degrees east) to screen X position (0 left -> 159 right) */
    x = X_CENTER + (unsigned char) ((lon << 2) / 9);
  
    /* Map latitude (-90 -> 90 degrees north) to screen Y position (0 top -> 79 bottom) */
    y = Y_CENTER - (unsigned char) ((lat << 2) / 9);
  
    /* Note: Both calculations are (N / 2.25):
       * -180 -> 180 is 360 degrees, mapped to 160 horizontal positions,
       * -90 -> 90 is 180 degress, mapped to 80 vertical positions
       This is being done in integer math as ((N * 4) / 9)
    */
  
    /* Top of screen is at PMG vertical position 16,
       so we want that to be the /center/ of where we draw the sprite */
    memcpy(pmg_mem + 512 + 16 - (SPRITE_HEIGHT / 2) + y, sprite, SPRITE_HEIGHT);
  
    /* Left of screen is at PMG horizontal position 48 */
    GTIA_WRITE.hposp0 = 48 - (SPRITE_WIDTH / 2) + x;
 
    OS.ch = KEY_NONE; 
    do {
      OS.pcolr0 = ((OS.rtclok[2] >> 1) & 0x0F) + 16;
      key = OS.ch;
    } while (key == KEY_NONE);
    OS.ch = KEY_NONE;

  } while (key != KEY_ESC);
  return;
}
