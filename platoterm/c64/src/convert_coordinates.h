/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Nick Shcherbyna
 *
 * convert_coordinates.c - Display output functions (base)
 */
#ifndef CONVERT_COORDINATES_H
#define CONVERT_COORdINATES_H

#include <stdint.h>
#include <stdio.h>


/* Multiply by 1.6 = 8/5 */
uint16_t touch_scale_320(uint16_t x);

/* Multiply by 2.(6) = 8/3 */
uint16_t touch_scale_192(uint16_t y);

/* Multiply by 2 = n<<4 + 12 for Apple2 */
uint16_t touch_scale_256(uint16_t x);

#endif /* CONVERT_COORDINATES_H */
