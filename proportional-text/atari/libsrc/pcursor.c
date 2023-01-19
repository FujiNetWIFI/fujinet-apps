/**
 * Proportional Font text output library for Atari
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

/** 
 * @var _pcx Cursor X position
 */
unsigned short _pcx;

/** 
 * @var _pcx Cursor Y position
 */
unsigned char _pcy;

/**
 * @brief set cursor to X,Y
 * @param x new X position (0-319)
 * @param y new Y position (0-191)
 */
void pgotoxy(unsigned short x, unsigned char y)
{
  _pcx = x;
  _pcy = y;
}
