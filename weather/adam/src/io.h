/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "options.h"
#include "location.h"

bool io_options_load(OptionsData *o);
bool io_options_save(OptionsData *o);
bool io_location_load(Location *o);
bool io_location_save(Location *o);
bool io_location_get_from_ip(char *c);
bool io_weather(char *j);
bool io_forecast(char *j);

#endif /* IO_H */
