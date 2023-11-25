/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "options.h"
#include "screen.h"
#include "io.h"

OptionsData optData;

bool options_load(OptionsData *o)
{
  return io_options_load(o);
}

void options_save(OptionsData *o)
{
  if (!io_options_save(o))
    screen_options_could_not_save();
}

void options_defaults(void)
{
  screen_options_init_not_found();

  memset(optData.apiKeyOW,0,sizeof(optData.apiKeyOW));
  strcpy(optData.apiKeyOW,"2e8616654c548c26bc1c86b1615ef7f1");
  optData.refreshInterval=DEFAULT_REFRESH;
  optData.units=UNKNOWN;
  optData.showRegion=false;
  optData.detectLocation=true;
  optData.maxPrecision=1;
  strcpy(optData.theme,"DEFAULT.THM");
  options_save(optData);
}

void options_init(void)
{
  screen_options_init();

  if (!options_load(optData))
    options_defaults();
}

void options(void)
{
  options_init();
}
