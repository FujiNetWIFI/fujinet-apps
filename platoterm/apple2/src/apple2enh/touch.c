/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions (apple2)
 */

#include "../protocol.h"
#include "../convert_coordinates.h"
#include <stdint.h>
#include "../config.h"

/**
 * touch_translate - Translate coordinates from native system to PLATO
 */
void touch_translate(padPt* Coord)
{
  Coord->x = touch_scale_256(Coord->x);
  Coord->y = touch_scale_192(Coord->y);
}

/**
 * touch_driver_name() - Get the driver name for the given driver #
 */
const char* touch_driver_name(unsigned char driver)
{
  switch(driver)
    {
    case CONFIG_MOUSE_DRIVER_STDMOU:
      return "a2.stdmou.mou";
    }
}
