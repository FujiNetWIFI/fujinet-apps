/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose ip geolocation
 */

#include <vic20.h>
#include <conio.h>
#include <string.h>
#include "defines.h"
#include "video.h"
#include "json.h"
#include "location.h"

extern State state;

Location l;

void location_get(void)
{
  json_init();
  cbm_open(LFN,DEV,SAN,IP_API);

  json_parse();

  json_query("/longitude");
  cbm_read(LFN,l.longitude,sizeof(l.longitude));
  
  json_query("/latitude");
  cbm_read(LFN,l.latitude,sizeof(l.latitude));
  
  json_query("/city");
  cbm_read(LFN,l.city,sizeof(l.city));
  
  json_query("/country_code");
  cbm_read(LFN,l.country_code,sizeof(l.country_code));
  
  json_query("/region_code");
  cbm_read(LFN,l.region_code,sizeof(l.region_code));
  
  cbm_close(LFN);
  json_done();
}

void location(void)
{
  clrscr();
  VIC.bg_border_color=254;

  revers(1);
  print("   FINDING LOCATION        PLEASE  WAIT     ");
  revers(0);

  location_get();
  
  print("\nLON: ");
  print(l.longitude);
  print("\n");

  print("LAT: ");
  print(l.latitude);
  print("\n");

  print("CTY: ");
  print(l.city);
  print("\n");

  print("COU: ");
  print(l.country_code);
  print("\n");

  print("REG: ");
  print(l.region_code);
  print("\n\nPRESS ANY KEY");

  cgetc();
  
  state=WEATHER;
}
