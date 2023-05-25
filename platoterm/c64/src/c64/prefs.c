/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions - C64 specific
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../prefs.h"
#include "../config.h"

extern ConfigInfo config;
extern uint8_t ch;
extern uint8_t prefs_need_updating;
extern uint8_t touch_prefs_updated;
extern uint8_t io_prefs_updated;
extern uint8_t io_load_successful;

/**
 * Show PLATOTERM READY - Press '<key>' for setup.
 */
void prefs_show_greeting(void)
{
  if (io_load_successful==false)
    prefs_display("serial driver didn't load. check device.");
  else
    prefs_display("platoterm 1.3 ready - f3 for setup");
}

/**
 * prefs_touch(void)
 * Preferences for selecting touch driver
 */
void prefs_touch(void)
{
  prefs_display("1)351 i)nkwell j)oy k)oala b)ack: ");

  ch=prefs_get_key_matching("1ijkbIJKB");

  switch(ch)
    {
    case '1':
      prefs_select("1351");
      config.driver_mou=CONFIG_MOUSE_DRIVER_1351;
      break;
    case 'i':
      prefs_select("inkwell");
      config.driver_mou=CONFIG_MOUSE_DRIVER_INKWELL;
      break;
    case 'j':
      prefs_select("joystick");
      config.driver_mou=CONFIG_MOUSE_DRIVER_JOY;
      break;
    case 'k':
      prefs_select("koala");
      config.driver_mou=CONFIG_MOUSE_DRIVER_POT;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
  prefs_check_for_touch_change();
}

/**
 * prefs_driver(void)
 * Preferences menu to select serial driver
 */
void prefs_driver(void)
{
  prefs_display("driver - s)wiftlink232 u)serport2400 b)ack: ");
  
  ch=prefs_get_key_matching("subSUB");

  switch(ch)
    {
    case 's':
      prefs_select("swiftlink232");
      config.driver_ser=CONFIG_SERIAL_DRIVER_SWIFTLINK;
      break;
    case 'u':
      prefs_select("userport2400");
      config.driver_ser=CONFIG_SERIAL_DRIVER_UP2400;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
  prefs_check_for_io_change();
}

