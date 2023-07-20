/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Board routines
 */

#include <atari.h>
#include <string.h>
#include "constants.h"
#include "board_pmg.h"
#include "charset.h"
#include "dlist.h"
#include "pfdata.h"
#include "typedefs.h"
#include "cursor.h"

/**
 * @brief main program state
 */
extern State state;

/**
 * @brief return color at board position
 * @brief x horizontal position (0-3)
 * @brief y vertical position (0-7)
 * @return Piece (see typedef.h)
 */
Piece board_get(unsigned char x, unsigned char y)
{
  return (Piece)BOARDPF[_board_pos[y][x]];
}

/**
 * @brief set piece on board
 * @param x horizontal position (0-3)
 * @parma y vertical position (0-7)
 * @param Piece (see typedef.h)
 */
void board_set(unsigned char x, unsigned char y, Piece piece)
{
  BOARDPF[_board_pos[y][x]] = piece;
}

/**
 * @brief Reset board piece positions
 */
void board_reset(void)
{
  memset((void *)BOARDPF_ADDR, 0x00, sizeof(_board_pfdata));
  memcpy((void *)BOARDPF_ADDR, &_board_pfdata, sizeof(_board_pfdata));
  cursor_pos(3,7);
}

/**
 * @rief set up board character set
 */
void board_charset(void)
{
  memset((void *)CHSET_ADDR, 0x00, sizeof(checkers_fnt));
  memcpy((void *)CHSET_ADDR, checkers_fnt, sizeof(checkers_fnt));
  OS.chbas = 0x7A;
}

/**
 * @brief set up board player/missile graphics
 */
void board_pmg(void)
{
  memset((void *)PMG_ADDR, 0x00, 1024);
  memcpy((void *)PMG_ADDR, &_pmg_data, sizeof(_pmg_data));
  GTIA_WRITE.gractl = GRACTL;
  OS.sdmctl = SDMCTL;
  ANTIC.pmbase = 0x7C;
}

/**
 * @brief set up display list for board
 * @verbose Display list is at 0x0600
 */
void board_dlist(void)
{
  memset((void *)DLIST_ADDR,0x00,256);
  memcpy((void *)DLIST_ADDR, &_dlist, sizeof(_dlist));
  OS.sdlst = (void *)DLIST_ADDR;
}

/**
 * @brief set up board color palette
 */
void board_color(void)
{
  // Checkers board background
  OS.pcolr0=
    OS.pcolr1=
    OS.pcolr2=
    OS.pcolr3=PCOLR;

  // Playfield colors
  OS.color0 = COLOR0;
  OS.color1 = COLOR1;
  OS.color2 = COLOR2;
  OS.color3 = COLOR3;
  OS.color4 = COLOR4;

  // Set P/M's behind playfield (pieces)
  OS.gprior = GPRIOR;

  // Position players and missiles (board bits)
  GTIA_WRITE.hposp0=HPOSP0;
  GTIA_WRITE.hposp1=HPOSP1;
  GTIA_WRITE.hposp2=HPOSP2;
  GTIA_WRITE.hposp3=HPOSP3;
  GTIA_WRITE.hposm0=HPOSM0;
  GTIA_WRITE.hposm1=
    GTIA_WRITE.hposm2=
    GTIA_WRITE.hposm3=HPOSM1;  
  GTIA_WRITE.sizep0=
    GTIA_WRITE.sizep1=
    GTIA_WRITE.sizep2=
    GTIA_WRITE.sizep3=SIZEP0;
  GTIA_WRITE.sizem=SIZEM;
}

/**
 * @brief draw board
 */
void board(void)
{
  board_dlist();
  board_color();
  board_pmg();
  board_charset();
  board_reset();
  state++;
}
