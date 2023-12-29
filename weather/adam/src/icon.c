/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <string.h>
#include "icon.h"
#include "constants.h"

unsigned char icon_get(char *c)
{
  if (strcmp(c,"01d") == 0 || strcmp(c,"01n") == 0)
    return ICON_CLEAR_SKY;
  else if (strcmp(c,"02d") == 0 || strcmp(c,"02n") == 0)
    return ICON_FEW_CLOUDS;
  else if (strcmp(c,"03d") == 0 || strcmp(c,"03n") == 0)
    return ICON_SCATTERED_CLOUDS;
  else if (strcmp(c,"04d") == 0 || strcmp(c,"04n") == 0)
    return ICON_BROKEN_CLOUDS;
  else if (strcmp(c,"09d") == 0 || strcmp(c,"09n") == 0)
    return ICON_SHOWER_RAIN;
  else if (strcmp(c,"10d") == 0 || strcmp(c,"10n") == 0)
    return ICON_RAIN;
  else if (strcmp(c,"11d") == 0 || strcmp(c,"11n") == 0)
    return ICON_THUNDERSTORM;
  else if (strcmp(c,"13d") == 0 || strcmp(c,"13n") == 0)
    return ICON_SNOW;
  else if (strcmp(c,"50d") == 0 || strcmp(c,"50n") == 0)
    return ICON_MIST;
}
