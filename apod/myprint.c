/*
  myprint.c

  2021-03-27 - 2021-04-20
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
