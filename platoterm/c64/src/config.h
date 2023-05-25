/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.h - Configuration routines
 */

#ifndef CONFIG_H
#define CONFIG_H

#define XON_THRESHOLD 46
#define XOFF_THRESHOLD 127

/* C64/128 Driver defines */
#define CONFIG_MOUSE_DRIVER_1351 0
#define CONFIG_MOUSE_DRIVER_JOY 1
#define CONFIG_MOUSE_DRIVER_INKWELL 2
#define CONFIG_MOUSE_DRIVER_POT 3
#define CONFIG_SERIAL_DRIVER_SWIFTLINK 0
#define CONFIG_SERIAL_DRIVER_UP2400 1

/* Atari Driver defines */
#define CONFIG_SERIAL_DRIVER_ATRRDEV 0
#define CONFIG_MOUSE_DRIVER_ATRAMI 0
#define CONFIG_MOUSE_DRIVER_ATRJOY 1
#define CONFIG_MOUSE_DRIVER_ATRST 2
#define CONFIG_MOUSE_DRIVER_ATRTRK 3
#define CONFIG_MOUSE_DRIVER_ATRTT 4

/* Apple2 Driver Defines */
#define CONFIG_SERIAL_DRIVER_SSC 0
#define CONFIG_MOUSE_DRIVER_STDMOU 0

#define CONFIG_MOUSE_DRIVER_NONE 0xFF

typedef struct configInfo
{
  unsigned char baud;
  unsigned char driver_ser;
  unsigned char driver_mou;
  unsigned char color_foreground;
  unsigned char color_background;
  unsigned char color_border;
  unsigned short xon_threshold;
  unsigned short xoff_threshold;
  unsigned char fill;
#if defined(__APPLE2__) || defined (__APPLE2ENH__)
  unsigned char slot;
#endif
} ConfigInfo;

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void);

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void);

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void);

/**
 * config_set_defaults()
 * Set default values for config file
 */
void config_set_defaults(void);

#endif /* CONFIG_H */
