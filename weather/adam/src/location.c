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
#include <conio.h>
#include "faux_json.h"
#include "location.h"
#include "options.h"
#include "screen.h"
#include "io.h"
#include "constants.h"

extern OptionsData optData;
Location locData;

static char tmp[256];

static const char *imperialCCodes[] =
  {"US","GB","IN","IE","CA","AU","HK","NZ"};

extern State state;

bool location_load(void)
{
  return io_location_load(&locData);
}

bool location_save(void)
{
    if (!io_location_save(&locData))
      screen_options_could_not_save();
}

bool locationIsImperial(void)
{
  unsigned char i=0;

  for (i=0;i<sizeof(imperialCCodes);i++)
    if (strncmp(locData.country_code,imperialCCodes[i],2) == 0)
      return true;

  return false;
}

void location_parse(char *j)
{
  strcpy(json,j);

  faux_parse_json("\"latitude\": ",0);
  strncpy(locData.latitude,json_part,7);

  faux_parse_json("\"longitude\": ",0);
  strncpy(locData.longitude,json_part,7);
  
  faux_parse_json("\"city\": ",0);
  strncpy(locData.city,json_part,40);

  faux_parse_json("\"country_code\": ",0);
  strncpy(locData.country_code,json_part,2);
  
  faux_parse_json("\"region_code\": ",0);
  strncpy(locData.region_code,json_part,2);
  
  cprintf(" latitude: %7s\n",locData.latitude);
  cprintf("longitude: %7s\n",locData.longitude);
  cprintf("     city: %s\n",locData.city);
  cprintf("   region: %2s\n",locData.region_code);
  cprintf("  country: %2s\n\n",locData.country_code);

  faux_parse_json("\"ip\": ",0);
  cprintf("       ip: %s",json_part);
}

void location_detect(void)
{
  screen_location_detect();
  io_location_get_from_ip(tmp);
  location_parse(tmp);

  if (optData.units == UNKNOWN)
    {
      if (locationIsImperial())
	optData.units = IMPERIAL;
      else
	optData.units = METRIC;
    }
  
  location_save();
  options_save(optData);
}

void location(void)
{
  if (!location_load())
    {
      if (optData.detectLocation == true)
	location_detect();
    }
  
  if (locData.latitude[0] != 0x00)
    state=WEATHER;
}
