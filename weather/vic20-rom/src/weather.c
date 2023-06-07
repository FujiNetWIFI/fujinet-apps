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

WeatherData wd;

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

void weather_icon(const char *s)
{
  if (s[0]=='0')
    {
      if (s[1]=='1')
	{
	  weather_icon_display(i01,false);
	}
      else if (s[1]=='2')
	{
	  weather_icon_display(i02,false);
	}
      else if (s[1]=='3')
	{
	  weather_icon_display(i03,false);
	}
      else if (s[1]=='4')
	{
	  weather_icon_display(i04,false);
	}
      else if (s[1]=='9')
	{
	  weather_icon_display(i04,true);
	}
    }
  else if (s[0]=='1')
    {
      if (s[1]=='0')
	{
	  weather_icon_display(i02,true);
	}
      else if (s[1]=='1')
	{
	  weather_icon_display(i11,true);
	}
      else if (s[1]=='3')
	{
	  weather_icon_display(i13,false);
	}
    }
  else if (s[0]=='5')
    {
      if (s[1]=='0')
	{
	  weather_icon_display(i50,false);
	}
    }
}

void weather_set_endpoint(void)
{
  memset(weather_url,0,sizeof(weather_url));
  strcpy(weather_url,weather_base_url);
}

void weather_set_location(void)
{
  // seriously, these character translation issues are driving me nuts -thom
  if (l.country_code[0] == 0x75)
    {
      if (l.country_code[0] == 0x73)
	{
	  units=IMPERIAL;
	  strcat(weather_url,"&units=imperial");
	}
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
  dt(wd.date_txt,"%Y-%m-%d",val);
  dt(wd.time_txt,"%H:%M",val);

  json_query("/current/sunrise");
  cbm_read(LFN,val,sizeof(val));
  dt(wd.sunrise_txt,"%H:%M",val);

  json_query("/current/sunset");
  cbm_read(LFN,val,sizeof(val));
  dt(wd.sunset_txt,"%H:%M",val);

  json_query("/current/temp");
  cbm_read(LFN,wd.feels_like,sizeof(wd.feels_like));
  strcat(wd.feels_like,"@");
  if (units==IMPERIAL)
    strcat(wd.feels_like,"F");
  else
    strcat(wd.feels_like,"C");

  json_query("/current/pressure");
  cbm_read(LFN,wd.pressure,sizeof(wd.pressure));
  strcat(wd.pressure," hPa");

  json_query("/current/humidity");
  cbm_read(LFN,wd.humidity,sizeof(wd.humidity));
  strcat(wd.humidity,"%");

  json_query("/current/dew_point");
  cbm_read(LFN,wd.dew_point,sizeof(wd.dew_point));

  json_query("/current/clouds");
  cbm_read(LFN,wd.clouds,sizeof(wd.clouds));
  strcat(wd.clouds,"%");

  json_query("/current/visibility");
  cbm_read(LFN,wd.visibility,sizeof(wd.visibility));
  if (strlen(wd.visibility)<5)
    wd.visibility[2]=0x00;
  else
    wd.visibility[1]=0x00;
  strcat(wd.visibility,"km");

  json_query("/current/wind_speed");
  cbm_read(LFN,wd.wind_speed,sizeof(wd.wind_speed));
  if (units==IMPERIAL)
    strcat(wd.wind_speed,"mph");
  else
    strcat(wd.wind_speed,"kph");

  json_query("/current/wind_deg");
  cbm_read(LFN,wd.wind_dir,sizeof(wd.wind_dir));
  deg=atoi(wd.wind_dir);
  weather_deg(wd.wind_dir);

  json_query("/current/weather/0/icon");
  cbm_read(LFN,wd.icon,sizeof(wd.icon));

  json_query("/current/weather/0/description");
  cbm_read(LFN,wd.description,sizeof(wd.description));
}

void weather_clear(void)
{
  memset(&wd,0,sizeof(WeatherData));
}

void weather_get(void)
{
  weather_clear();
  weather_set_endpoint();
  weather_set_location();
  json_init();
  cbm_open(LFN,DEV,SAN,weather_url);
  json_parse();
  weather_query();
  cbm_close(LFN);
  json_done();
}

void weather(void)
{
  weather_get();
  clrscr();
  color_setup();
  
  // Date/time
  gotoxy(1,1);
  print(wd.date_txt);
  print("  ");
  print(wd.time_txt);

  // Icon
  y=3;
  x=17;
  weather_icon(wd.icon);
  gotoxy(x-7,y);
  print(wd.feels_like);

  // city
  gotoxy(1,y);
  print(l.city);
  print("\n ");
  print(l.region_code);
  print(" ");
  print(l.country_code);
  
  gotoxy(0,y+4);

  print(" PRESSURE: ");
  print(wd.pressure);
  print("\n\n");
  
  print(" HUMIDITY: ");
  print(wd.humidity);
  print("\n\n");

  print("   DEW PT: ");
  print(wd.dew_point);
  print("\n\n");

  print("   CLOUDS: ");
  print(wd.clouds);
  print("\n\n");

  print("VISIBILTY: ");
  print(wd.visibility);
  print("\n\n");

  print("     WIND: ");
  print(wd.wind_speed);
  print(" ");
  print(wd.wind_dir);

  gotoxy(0,19);

  print("  SUNRISE: ");
  print(wd.sunrise_txt);
  print("\n\n");

  print("   SUNSET: ");
  print(wd.sunset_txt);

  wait=65535;

  while (wait--)
    {
      if (kbhit())
	{
	  cgetc();
	  return;
	}
      
      waitvsync();
    }
  
}
