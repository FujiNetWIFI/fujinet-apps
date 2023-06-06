/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose weather
 */

#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json.h"
#include "defines.h"
#include "location.h"
#include "icons.h"
#include "video.h"
#include "color.h"

extern State state;
extern Location l;

WeatherData wd[3];

unsigned char x,y; // Screen coords
unsigned short wait; // vblanks
Unit units;

const char weather_base_url[] = "http://api.openweathermap.org/data/2.5/onecall?exclude=minutely,hourly,alerts,daily&appid=2e8616654c548c26bc1c86b1615ef7f1";
char weather_url[200];

void weather_icon_display(const char *s, bool storm)
{
  unsigned char o;
  
  o = y*22+x;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[0],4);
  
  o += 22;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[4],4);

  o += 22;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[8],4);

  if (storm)
    {
      o += 22;
      memset(&COLOR_RAM[o],0x0b,4);
      memcpy(&SCREEN_RAM[o],&s[12],4);
    }
}

void weather_set_endpoint(void)
{
  memset(weather_url,0,sizeof(weather_url));
  strcpy(weather_url,weather_base_url);
}

void weather_set_location(void)
{
  if (l.country_code[0] == 'U' && l.country_code[1] == 'S')
    {
      units=IMPERIAL;
      strcat(weather_url,"&units=imperial");
    }
  else
    {
      units=METRIC;
      strcat(weather_url,"&units=metric");
    }

  strcat(weather_url,"&lon=");
  strcat(weather_url,l.longitude);
  strcat(weather_url,"&lat=");
  strcat(weather_url,l.latitude);
}

void weather_get(void)
{
}

void weather_test(void)
{
}

void weather(void)
{
  weather_get();
  clrscr();
  color_setup();
  
  // Date/time
  gotoxy(1,1);
  print(wd[0].date_txt);
  print("  ");
  print(wd[0].time_txt);

  // Icon
  y=3;
  x=17;
  weather_icon_display(i02,false);
  gotoxy(x-4,y);
  print(wd[0].feels_like);

  // city
  gotoxy(1,y);
  print(l.city);
  print("\n ");
  print(l.region_code);
  print(" ");
  print(l.country_code);
  
  gotoxy(0,y+4);

  print("  PRESSURE: ");
  print(wd[0].pressure);
  print("\n\n");
  
  print("  HUMIDITY: ");
  print(wd[0].humidity);
  print("\n\n");

  print("    DEW PT: ");
  print(wd[0].dew_point);
  print("\n\n");

  print("    CLOUDS: ");
  print(wd[0].clouds);
  print("\n\n");

  print(" VISIBILTY: ");
  print(wd[0].visibility);
  print("\n\n");

  print("      WIND: ");
  print(wd[0].wind_speed);
  print(" ");
  print(wd[0].wind_dir);
  print("\n\n");

  print("   SUNRISE: ");
  print(wd[0].sunrise_txt);
  print("\n\n");

  print("    SUNSET: ");
  print(wd[0].sunset_txt);

  wait=65535;

  while (wait--)
    {
      if (kbhit())
	{
	  switch (cgetc())
	    {
	    case CH_F1:
	      state=FORECAST;
	      return;
	    case CH_F2:
	      units^=units;
	      return;
	    }
	}
      
      waitvsync();
    }
  
}
