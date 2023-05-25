/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * config.c - Configuration routines
 */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "screen.h"

#ifndef __ATARI__
#define CONFIG_FILE "config"
#endif

ConfigInfo config;

/* This whole block of code is redefined on the atari to use nothing but cio calls. */
#ifndef __ATARI__ 

/**
 * config_init()
 * Initialize configuration and load either config or defaults.
 */
void config_init(void)
{
  memset(&config,0,sizeof(config));
  config_load();
  screen_update_colors(); /* because the screen is already initialized. */
}

/**
 * config_load()
 * Load the configuration file, or if not found, set some defaults and save.
 */
void config_load(void)
{
  FILE *fp;
  fp = fopen(CONFIG_FILE,"r");
  
  if (!fp)
      config_set_defaults();
  else
      fread(&config,1,sizeof(config),fp);

  fclose(fp);
}

/**
 * config_save()
 * Save the configuration file.
 */
void config_save(void)
{
  FILE *fp;
  fp = fopen(CONFIG_FILE,"w");

  if (!fp)
    return;

  fwrite(&config,1,sizeof(config),fp);
  fclose(fp);
}

#endif /* if __ATARI__ is not defined */
