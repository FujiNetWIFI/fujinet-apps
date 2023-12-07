/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose ANTIC display list
 */

static const void _dlist =
  {
    DL_BLK8,
    DL_BLK8,
    DL_BLK5,
    
    DL_LMS(DL_CHR20x8x2),
    MSGPF_ADDR,
    DL_BLK2,
    
    DL_LMS(DL_CHR20x8x2),
    BOARDPF_ADDR,
    DL_BLK2,

    DL_CHR20x16x2,
    DL_BLK6,    
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,
    DL_BLK6,
    DL_CHR20x16x2,

    DL_BLK2,
    DL_CHR20x8x2,
    
    DL_JVB,
    DLIST_ADDR
  };
