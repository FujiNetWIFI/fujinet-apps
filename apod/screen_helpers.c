/*
  screen_helpers.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module provides some helpers for managing the
  screen, including blanking ANTIC and clearing screen memory,
  enabling ANTIC (DMA), waiting for the next VBLANK
  (to reduce flicker/tearing while doing things),
  and setting up pointers to the various screen and Display List
  memory blocks.

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2023-12-29
*/

#include <string.h> /* for memset() */
#include <atari.h>
#include "screen_helpers.h"

extern unsigned char scr_mem[];
unsigned char * scr_mem1, * scr_mem2, * scr_mem3;
unsigned char * dlist1, * dlist2, * dlist3;

/**
 * Disable ANTIC; clear screen & display list memory
 * (Note: `txt_mem` buffer resides at the end of the
 * 24K block that `scr_mem` starts at)
 */
void screen_off(void) {
  OS.sdmctl = 0;
  bzero(scr_mem, (SCR_BLOCK_SIZE * 3));
//memset(scr_mem, 0, (SCR_BLOCK_SIZE * 3)); /* optimizes into 'bzero', but then we get an error: screen_helpers.s:64: Error: Unresolved external '___bzero' -bjk 2023.12.29 */
}

/**
 * Point to display list; re-enable ANTIC
 */
void screen_on(void) {
  OS.sdlst = dlist1;
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH;
}

/**
 * Wait a moment (so screen can come to life before
 * #FujiNet takes over, if we're fetching from the network).
 */
void wait_for_vblank(void) {
  char frame = (OS.rtclok[2] + 2);
  while ((OS.rtclok[2] + 2) == frame);
}

void set_screen_and_dlist_pointers(void) {
  scr_mem1 = (unsigned char *) (scr_mem + SCR_OFFSET);
  dlist1 = (unsigned char *) (scr_mem1 + DLIST_OFFSET);
  scr_mem2 = (unsigned char *) (scr_mem + SCR_BLOCK_SIZE + SCR_OFFSET);
  dlist2 = (unsigned char *) (scr_mem2 + DLIST_OFFSET);
  scr_mem3 = (unsigned char *) (scr_mem + (SCR_BLOCK_SIZE * 2) + SCR_OFFSET);
  dlist3 = (unsigned char *) (scr_mem3 + DLIST_OFFSET);
}
