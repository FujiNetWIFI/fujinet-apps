/**
 * Clock
 *
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <stdio.h>
#include <conio.h>
#include "sp.h"

unsigned char clock_device;

void main(void)
{
  unsigned char *datetime;

  clrscr();
  sp_init();
  clock_device = sp_find_clock();

  // Directly using Smartport
  sp_status(clock_device, 'T'); // Get Status
  printf("\n\nDirectly from Fujinet:\n    Date: %02u%02u-%02u-%02u\n    Time: %02u:%02u:%02u\n",
    sp_payload[0], // Century
    sp_payload[1], // Year
    sp_payload[2], // Month
    sp_payload[3], // Day
    sp_payload[4], // Hour
    sp_payload[5], // Minute
    sp_payload[6]); // Second

  // Using the prodos driver
    __asm__ volatile ("lda #%b", (char)0x82 /* GET_TIME */);
    __asm__ volatile ("sta %g", mliCmd); // store status command #

    __asm__ volatile ("jsr $BF00");
mliCmd:
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");

  datetime = (unsigned char *)0xbf90;
  printf("\n\nUsing the Prodos call and driver:\n    Date: %02u-%02u-%02u\n    Time: %02u:%02u\n",
    datetime[1] >> 1 , // Year
    ((datetime[1] & 0x1) << 3) + (datetime[0] >> 5), // Month
    datetime[0] & 0x1f, // Day
    datetime[3], // Hour
    datetime[2]); // Minute
  while(1) {}
};
