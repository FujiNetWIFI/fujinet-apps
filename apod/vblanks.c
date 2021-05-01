/*
  vblanks.c

  By Bill Kendrick <bill@newbreedsoftware.com>
  2021-03-27 - 2021-05-01
*/

#include <stdio.h>
#include <atari.h>
#include "dli15.h"
#include "dli256.h"
#include "dli9.h"
#include "rgb.h"
#include "interrupt_helpers.h"

/* FIXME: This module is 100% assembly language,
   so should be converted from a .c file to a .s source.
   In the meantime, it works, so not gonna bother yet.
   -bjk 2021.05.01 */

/* Tracking which Display List is active */
unsigned char dlist_hi, dlist_lo;

/* Keeping track of which RGB color we're showing
   (for fetching from the look-up table),
   and APAC frame, and luminence */
unsigned char rgb_ctr, apac_scanline, apac_lum;


#pragma optimize (push, off)

/* VBI routine for flipping between our three
   Display Lists in RGB image modes (ColorView9) */
void VBLANKD9(void) {
  /* grab the current rgb color counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 3 back to 0 */
  asm("inx");
  asm("cpx #3");
  asm("bcc %g", __vbi9_ctr_set);

  asm("ldx #0");

__vbi9_ctr_set:
  /* store the current rgb color counter back;
     also store it as a reference to our next display list */
  asm("stx %v", dli9_load_arg);
  asm("stx %v", rgb_ctr);

  /* display lists are 8K away from each other
     (tucked under screen memory); that's 32 (256 byte) pages,
     so we can shift left 5 times to multiply the rgb color counter
     by 32... then store it in the high byte of SDLST */
  asm("txa");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("adc %v", dlist_hi);
  asm("sta $d403");
  asm("lda %v", dlist_lo);
  asm("sta $d402");

  /* adjust end of the screen colors - set the last one to black color */
  asm("lda %v+187,x", rgb_table);
  asm("sta %v+190,x", rgb_table);
  asm("lda %v+188,x", rgb_table);
  asm("sta %v+191,x", rgb_table);
  asm("lda #0");
  asm("sta %v+192,x", rgb_table);

  /* start next screen with black color at top */
  asm("sta $d01a");

  asm("jmp (%v)", OLDVEC);
}

/* VBI routine for flipping between our three
   Display Lists in RGB image modes (ColorView15) */
void VBLANKD15(void) {
  /* grab the current rgb color counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 3 back to 0 */
  asm("inx");
  asm("cpx #3");
  asm("bcc %g", __vbi15_ctr_set);

  asm("ldx #0");

__vbi15_ctr_set:
  /* store the current rgb color counter back */
  asm("stx %v", rgb_ctr);

  /* display lists are 8K away from each other
     (tucked under screen memory); that's 32 (256 byte) pages,
     so we can shift left 5 times to multiply the rgb color counter
     by 32... then store it in the high byte of SDLST */
  asm("txa");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("adc %v", dlist_hi);
  asm("sta $d403");
  asm("lda %v", dlist_lo);
  asm("sta $d402");

  asm("jmp (%v)", OLDVEC);
}

/* VBI routine for flipping between our two
   Display Lists in APAC image modes */
void VBLANKD_APAC(void) {
  /* grab the current counter */
  asm("ldx %v", rgb_ctr);

  /* increment it; roll from 2 back to 0 */
  asm("inx");
  asm("cpx #2");
  asm("bcc %g", __vbi256_ctr_set);

  asm("ldx #0");

__vbi256_ctr_set:
  /* store the current counter back;
     also store it as a reference to our next display list */
  asm("stx %v", rgb_ctr);

  /* display lists are 8K away from each other
     (tucked under screen memory); that's 32 (256 byte) pages,
     so we can shift left 5 times to multiply the rgb color counter
     by 32... then store it in the high byte of SDLST */
  asm("txa");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("asl a");
  asm("adc %v", dlist_hi);
  asm("sta $d403");
  asm("lda %v", dlist_lo);
  asm("sta $d402");

  asm("jmp (%v)", OLDVEC);
}

#pragma optimize (pop)

