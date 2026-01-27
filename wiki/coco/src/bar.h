/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Bar routines
 */

#ifndef BAR_H
#define BAR_H

#include <cmoc.h>
#include <coco.h>
#include "fujinet-fuji.h"

#define SCREEN_RAM_TOP_32 0x0400

/**
 * @brief toggle bar for Y position (0-15) 
 * @param y vertical position (0-15) 
 */
void bar(int y);

/**
 * @brief Draw a line of text across the screen 
 * @param y Vertical position (0-23)
 * @param fgcolor The foreground color
 * @param bgcolor The background color
 * @param text The text to print 
 * @param rev Whether to reverse the text
 */
void hd_bar(byte y, const char *text, bool rev);

/**
 * @brief Draw a multiple lines of text across the screen 
 * @param y Vertical position (0-23)
 * @param lines The number of consecutive lines to highlight
 * @param text The text to print 
 * @param rev Whether to reverse the text
 */
void multiline_hd_bar(byte y, int lines, const char *text, bool rev);

/**
 * @brief draw shadow for color c at vert pos y
 * @param y Vertical position (0-15)
 * @param c bkg color (0-7) 
 */
void shadow(int y, int c);

#endif /* BAR_H */
