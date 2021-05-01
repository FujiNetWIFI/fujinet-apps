/*
  myprint.c

  Astronomy Picture of the Day (APOD) viewer client
  for Ataris using #Fujinet.

  This module draws strings onto a text mode display
  (e.g., the main menu)

  2021-03-27 - 2021-05-01
*/

#include "myprint.h"

/**
 * Simple text rendering onto screen memory
 */
void myprint(unsigned char scr_mem[], unsigned char x, unsigned char y, char * str) {
  int pos, i;
  unsigned char c;

  pos = y * 20 + x;
  for (i = 0; str[i] != '\0'; i++) {
    c = str[i];

    if (c < 32) {
      c = c + 64;
    } else if (c < 96) {
      c = c - 32;
    }

    scr_mem[pos + i] = c;
  }
}
