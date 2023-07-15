/**
 * Functions to display a selection bar
 */

#include <atari.h>
#include <string.h>
#include "bar.h"

#define BAR_PMBASE	0x7c00
unsigned char* bar_pmbase=(unsigned char *)BAR_PMBASE;
static unsigned char bar_y=1;

/**
 * Clear bar from screen
 */
void bar_clear(void)
{
  memset(bar_pmbase,0,1024);
}

/**
 * Set bar color
 */
void bar_set_color(unsigned char c)
{
  OS.pcolr0=OS.pcolr1=OS.pcolr2=OS.pcolr3=c;
}

/**
 * Show bar at Y position
 */
void fastcall bar_show(unsigned char y)
{

  bar_y = y;

  asm ("asl");
  asm ("asl");
  asm ("adc #%b",16);
 
  asm ("pha");
  bar_clear();
  asm ("pla");
  asm ("tax");
  asm ("ldy #%b",14);
  asm ("lda #%b",-1);
lab:
  // First the missiles (rightmost portion of bar)
  asm ("sta %w,x",BAR_PMBASE+384);
  // Then the players (p0)
  asm ("sta %w,x",BAR_PMBASE+512);
  // P1
  asm ("sta %w,x",BAR_PMBASE+640);
  // P2
  asm ("sta %w,x",BAR_PMBASE+768);
  // P3
  asm ("sta %w,x",BAR_PMBASE+896);
  asm ("inx");
  asm ("dey");
  asm ("bne %g",lab);

}
