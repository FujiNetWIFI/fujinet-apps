/**
 * Battleship
 *
 * Display List Routines and data
 *
 */

#include <atari.h>
#include <string.h>
#include "dlist.h"
#include "screen.h"

extern unsigned char* dlist_ptr;

void dlist=
  {
   DL_BLK8,
   DL_BLK8,
   DL_BLK8,
   DL_LMS(DL_CHR40x8x1),
   DISPLAY_MEMORY,
   DL_CHR40x8x1,
   DL_DLI(DL_CHR40x8x1),
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_DLI(DL_CHR40x8x1),
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_CHR40x8x1,
   DL_JVB, 0x600
  };

/**
 * copy dlist into place and point SDLST to it.
 */
void dlist_setup(void)
{
  OS.sdmctl=0;
  memcpy((void *)DISPLAY_LIST, &dlist, sizeof(dlist));
  OS.sdlst=(void *)DISPLAY_LIST;
  dlist_ptr=(unsigned char *)OS.sdlst;
  OS.sdmctl=0x2e;
}
