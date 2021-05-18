#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "nsio.h"

/* A chunk of memory for the screen
   (bitmapped graphics, text window, and display list),
   as described in `atari.cfg`, and found in `segments.s` */
extern unsigned char scr_mem[];

/* Pointers within `scr_mem` */
unsigned char * dlist;
unsigned char * txt_mem;

/* The map bitmap */
extern unsigned char map_data[];

/* Set up the screen */
void setup(void) {
  unsigned char i;

  /* Turn off the screen (ANTIC DMA) */
  OS.sdmctl = 0;

  /* Set our pointers */
  txt_mem = scr_mem + (40 * 80);
  dlist = txt_mem + (40 * 4);

  /* Clear everything */
  bzero(scr_mem, (dlist - scr_mem + 1));

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

  /* Use our display list & enable the screen */
  OS.sdlst = dlist;
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH;
}

#define X_CENTER 79
#define Y_CENTER 39

void main(void) {
  int i;
  unsigned char n;
  int lat, lon;
  unsigned char x, y;

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

  /* Draw the ISS in its position */
  lon = 114;
  lat = -40;

  /* Map longitude (-180 -> 180 degrees east) to screen X position (0 left -> 159 right) */
  x = X_CENTER + (unsigned char) ((lon << 2) / 9);

  /* Map latitude (-90 -> 90 degrees north) to screen Y position (0 top -> 79 bottom) */
  y = Y_CENTER - (unsigned char) ((lat << 2) / 9);

  /* Note: Both calculations are (N / 2.25):
     * -180 -> 180 is 360 degrees, mapped to 160 horizontal positions,
     * -90 -> 90 is 180 degress, mapped to 80 vertical positions
     This is being done in integer math as ((N * 4) / 9)
  */

  do {
    POKE(scr_mem + y * 40 + (x / 4), 0);
    POKE(scr_mem + y * 40 + (x / 4) + 40, 0);
    POKE(scr_mem + y * 40 + (x / 4) - 40, 0);
    n = OS.rtclok[2] + 1;
    do { } while (OS.rtclok[2] != n);

    POKE(scr_mem + y * 40 + (x / 4), 255);
    POKE(scr_mem + y * 40 + (x / 4) + 40, 255);
    POKE(scr_mem + y * 40 + (x / 4) - 40, 255);
    n = OS.rtclok[2] + 1;
    do { } while (OS.rtclok[2] != n);
  } while(1);
  return;
}
