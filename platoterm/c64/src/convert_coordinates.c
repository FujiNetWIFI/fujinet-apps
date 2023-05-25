/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Nick Shcherbyna
 *
 * convert_coordinates.c - Display output functions (base)
 */

#include "convert_coordinates.h"

/* Multiply by 1.6 = 8/5 */
uint16_t touch_scale_320(uint16_t x)
{
    uint16_t n, q;
    n  = x << 3;
    q  = x + (x >> 1);
    q += q >> 4;
    q += q >> 8;
    n -= q << 2;
    n -= q;
    n += ((n << 1) + n) << 2;
    return q + (n >> 6);
}

/* Multiply by 2.(6) = 8/3 */
uint16_t touch_scale_192(uint16_t y)
{
    uint16_t n, q;
    n  = y << 3;
    q  = ((y << 1) + n) >> 2;
    q += q >> 4;
    q += q >> 8;
    n -= q << 1;
    n -= q;
    n += ((n << 2) + n) << 1;
    return (q + (n >> 5) ^ 0x1FF);
}

/* Multiply by 2 = n<<4 + 12 for Apple2 */
uint16_t touch_scale_256(uint16_t x)
{
  return ((x<<4)+12);
}
