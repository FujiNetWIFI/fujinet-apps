/**
 * @brief   News Reader
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Bar routines
 */

#ifndef BAR_H
#define BAR_H

/**
 * @brief toggle bar for Y position (0-15) 
 * @param y vertical position (0-15) 
 */
void bar(int y);

/**
 * @brief draw shadow for color c at vert pos y
 * @param y Vertical position (0-15)
 * @param c bkg color (0-7) 
 */
void shadow(int y, int c);

#endif /* BAR_H */
