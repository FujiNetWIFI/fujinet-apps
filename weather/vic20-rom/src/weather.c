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
#include "dt.h"

extern State state;
extern Location l;

WeatherData wd[3];

unsigned char x,y; // Screen coords
unsigned short wait; // vblanks
Unit units;
int deg;

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

void weather_deg(char *s)
{
  int i=deg/22;
  const char *d[16]={"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW"};
  strcpy(s,d[i]);
}

void weather_query(void)
{
  char val[32];
  
  memset(val,0,sizeof(val));
  
  json_query("/current/dt");
  cbm_read(LFN,val,sizeof(val));
  dt(wd[0].date_txt,"%Y-%m-%d",val);
  dt(wd[0].time_txt,"%H:%M",val);

  json_query("/current/sunrise");
  cbm_read(LFN,val,sizeof(val));
  dt(wd[0].sunrise_txt,"%H:%M",val);

  json_query("/current/sunset");
  cbm_read(LFN,val,sizeof(val));
  dt(wd[0].sunset_txt,"%H:%M",val);

  json_query("/current/temp");
  cbm_read(LFN,wd[0].feels_like,sizeof(wd[0].feels_like));
  strcat(wd[0].feels_like,"@");
  if (units==IMPERIAL)
    strcat(wd[0].feels_like,"F");
  else
    strcat(wd[0].feels_like,"C");

  json_query("/current/pressure");
  cbm_read(LFN,wd[0].pressure,sizeof(wd[0].pressure));
  strcat(wd[0].pressure," hPa");

  json_query("/current/humidity");
  cbm_read(LFN,wd[0].humidity,sizeof(wd[0].humidity));
  strcat(wd[0].humidity,"%");

  json_query("/current/dew_point");
  cbm_read(LFN,wd[0].dew_point,sizeof(wd[0].dew_point));

  json_query("/current/clouds");
  cbm_read(LFN,wd[0].clouds,sizeof(wd[0].clouds));
  strcat(wd[0].clouds,"%");

  json_query("/current/visibility");
  cbm_read(LFN,wd[0].visibility,sizeof(wd[0].visibility));
  if (strlen(wd[0].visibility)<5)
    wd[0].visibility[2]=0x00;
  else
    wd[0].visibility[1]=0x00;
  strcat(wd[0].visibility,"km");

  json_query("/current/wind_speed");
  cbm_read(LFN,wd[0].wind_speed,sizeof(wd[0].wind_speed));
  if (units==IMPERIAL)
    strcat(wd[0].wind_speed,"mph");
  else
    strcat(wd[0].wind_speed,"kph");

  json_query("/current/wind_deg");
  cbm_read(LFN,wd[0].wind_dir,sizeof(wd[0].wind_dir));
  deg=atoi(wd[0].wind_dir);
  weather_deg(wd[0].wind_dir);
}

void weather_get(void)
{
  weather_set_location();
  cbm_open(LFN,DEV,SAN,weather_url);
  json_parse();
  weather_query();
  cbm_close(LFN);
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
