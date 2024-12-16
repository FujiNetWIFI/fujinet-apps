/**
 * Bar routines
 */

#include <peekpoke.h>
#include <stdbool.h>
#include "bar.h"

unsigned char *ram = (unsigned char *)0x0000;

const unsigned short row[24]= {
			       0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700,
			       0x0780, 0x0428, 0x04A8, 0x0528, 0x05A8, 0x0628, 0x06A8, 0x0728, 0x07A8,
			       0x0450, 0x04D0, 0x0550, 0x05D0, 0x0650, 0x06D0, 0x0750, 0x07D0
};

/**
 * static local variables for bar y, max, and index.
 */
static unsigned char bar_y=3, bar_c=1, bar_m=1, bar_i=0, bar_oldi=0;

unsigned short bar_coord(unsigned char x, unsigned char y)
{
  return row[y]+x;
}

void bar_clear(unsigned char oldRow)
{
  char i;
  char by;

  if (oldRow)
    by = bar_y + bar_oldi;
  else
    by = bar_y + bar_i;

  for (i = 0; i < 40; i++)
    ram[bar_coord(i, by)] |= 0x80; // white char on black background is in upper half of char set
}

/**
 * Update bar display
 */
void bar_update(void)
{
  char i;
  
  bar_clear(true);
  
  // Clear bar color 
  for (i=0;i<40;i++)
    ram[bar_coord(i,bar_y+bar_i)] &= 0x3f; // black char on white background is in lower half of char set
}

/**
 * Set up bar and start display on row
 * @param y Y column for bar display
 * @param c column size in characters
 * @param m number of items
 * @param i item index to start display
 */
void bar_set(unsigned char y, unsigned char c, unsigned char m, unsigned char i)
{
  bar_y = y;
  bar_c = c;
  bar_m = m-1;
  bar_i = i;
  bar_oldi = bar_i;
  bar_update();
}

/**
 * Move bar upward until index 0
 */
void bar_up()
{
  bar_oldi=bar_i;
  
  if (bar_i > 0)
    {
      bar_i--;
      bar_update();
    }
}

/**
 * Move bar downward until index m
 */
void bar_down()
{
  bar_oldi=bar_i;

  if (bar_i < bar_m)
    {
      bar_i++;
      bar_update();
    }
}

/**
 * Jump to bar slot
 */
void bar_jump(unsigned char i)
{
  bar_oldi=bar_i;
  bar_i=i;
  bar_update();
}

/**
 * Get current bar position
 * @return bar index
 */
unsigned char bar_get()
{
  return bar_i;
}

