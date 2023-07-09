/**
 * @brief   Homesoft Launcher
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <atari.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>
#include "init.h"

#define CONSOLE_DISPLAY_LIST ((void *)0xBC40)

/**
 * @brief display list if BASIC is NOT enabled
 */
void dlist_nobasic =
  {
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    DL_LMS(DL_CHR20x8x2),
    0xBC40,
    DL_CHR20x8x2,
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
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR20x8x2,
    DL_CHR20x8x2,
    DL_JVB,
    0xBC40
  };

/**
 * @brief display list if BASIC is enabled
 */
void dlist_basic =
  {
    DL_BLK8,
    DL_BLK8,
    DL_BLK8,
    DL_LMS(DL_CHR20x8x2),
    0x9C40,
    DL_CHR20x8x2,
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
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR40x8x1,
    DL_CHR20x8x2,
    DL_CHR20x8x2,
    DL_JVB,
    0x9C40
  };

/**
 * @brief Set up the screen
 */
void init_screen(void)
{
  // Put display list into memory
  if (PEEK(0x9C20) == 0x70)
    memcpy((void *)0x9C20,&dlist_basic,sizeof(dlist_basic));
  else
    memcpy((void *)0xBC20,&dlist_nobasic,sizeof(dlist_nobasic));

  // Clear screen
  clrscr();
}

/**
 * @brief Connect to the Homesoft host
 */
void init_connect(void)
{

}

void init(void)
{
  init_screen();
  init_connect();
}
