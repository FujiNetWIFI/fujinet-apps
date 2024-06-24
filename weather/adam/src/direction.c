/**
 * Weather / direction.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */
#include "direction.h"

const char *directions[] = {"N ", "NE", "E ", "SE", "S ", "SW", "W ", "NW"};

const char *degToDirection(unsigned short a)
{
    unsigned short i = (a / 45) % 8;

    return directions[i];
}
