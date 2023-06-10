/**
 * @brief #FujiNet Reversi
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"
#include "translation.h"

/**
 * @brief is position selected?
 */
bool selected;

/**
 * @brief Translate Atari coordinates to board position
 * @param s String containing x,y,t
 * @return board position (0-63)
 */
Position translate_atari_to_position(char *s)
{
  unsigned char x,y;
  char *p = strtok(s,",");

  x = atoi(p);

  strtok(NULL,",");
  y = atoi(p);

  strtok(NULL,",");
  selected = atoi(p);
  
  x -= 22; // make first X position = 0
  y -= 5;  // make first Y position = 0

  x /= 5;  // scale X position to 0-7
  x /= 4;  // scale Y position to 0-7

  return (y*8) + x;
}

/**
 * @brief Translate position and selected into Atari coordinates
 */
void translate_position_to_atari(Position p, char *s)
{
  unsigned char y = p / BOARD_SIZE;
  unsigned char x = p % BOARD_SIZE;
  char tmp[4];
  
  // Clear destination string, min 10 chars.
  memset(s,0,8);

  itoa(x,tmp,10);
  strcpy(s,tmp);

  strcat(s,",");

  itoa(y,tmp,10);
  strcat(s,tmp);

  strcat(s,",");

  itoa(!selected,tmp,10);

  strcat(s,"\x9B");
}
