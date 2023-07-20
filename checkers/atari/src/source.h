/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Move Source routines
 */

#ifndef SOURCE_H
#define SOURCE_H

/**
 * @brief selected source horizontal (0-3)
 */
extern unsigned char source_x;

/**
 * @brief selected source vertical (0-7)
 */
extern unsigned char source_y;

/**
 * @brief select source square
 */
void source(void);

/**
 * @brief blink selected source square
 */
void source_blink(void);

#endif /* SOURCE_H */
