/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions (c64)
 */

#include "../protocol.h"
#include "../convert_coordinates.h"
#include "../config.h"

/**
 * touch_translate - Translate coordinates from native system to PLATO
 */
void touch_translate(padPt* Coord)
{
  Coord->x = touch_scale_320(Coord->x);
  Coord->y = touch_scale_192(Coord->y);
}

/**
 * touch_driver_name() - Get the driver name for the given driver #
 */
const char* touch_driver_name(unsigned char driver)
{
  switch(driver)
    {
    case CONFIG_MOUSE_DRIVER_1351:
      return "mou-1351";
    case CONFIG_MOUSE_DRIVER_JOY:
      return "mou-joy";
    case CONFIG_MOUSE_DRIVER_INKWELL:
      return "mou-inkwell";
    case CONFIG_MOUSE_DRIVER_POT:
      return "mou-pot";
    }
}
