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
  /* if (l.country_code[0] == 0x75) */
  /*   { */
  /*     if (l.country_code[0] == 0x73) */
  /* 	{ */
  /* 	  units=IMPERIAL; */
  /* 	  strcat(weather_url,"&units=imperial"); */
  /* 	} */
  /*   } */
  /* else */
  /*   { */
  /*     units=METRIC; */
  /*     strcat(weather_url,"&units=metric"); */
  /*   } */

  units=METRIC;
  strcat(weather_url,"&units=metric");
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

void weather_tod(char *val)
{
  int hr=0;
  char tmp[4];

  memset(tmp,0,sizeof(tmp));
  
  dt(tmp,"%H",val);
  hr = atoi(tmp);
  
  switch (hr)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      VIC.bg_border_color=9;
      cbm_k_bsout('\x05'); // white  
      break;
    case 5:
    case 6:
    case 7:
      VIC.bg_border_color=105;
      cbm_k_bsout('\x05'); // white  
      break;
    case 8:
    case 9:
    case 10:
      VIC.bg_border_color=57;
      cbm_k_bsout('\x90'); // black 
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
      VIC.bg_border_color=185;
      cbm_k_bsout('\x90'); // white  
      break;
    case 17:
    case 18:
    case 19:
      VIC.bg_border_color=105;
      cbm_k_bsout('\x05'); // white  
      break;
    case 20:
    case 21:
    case 22:
    case 23:
      VIC.bg_border_color=9;
      cbm_k_bsout('\x05'); // white  
      break;
    }

  VIC.volume_color=0x90; // aux = yellow

  // Go ahead and pre-paint the color on screen
  memset(COLOR_RAM,0x01,506);
}

void weather_query(void)
{
  char val[32];
  int i;

  memset(val,0,sizeof(val));
  
  json_query("/current/dt");
  cbm_read(LFN,val,sizeof(val));
  weather_tod(val);
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
  strcat(wd.dew_point,"@");
  if (units==IMPERIAL)
    strcat(wd.dew_point,"F");
  else
    strcat(wd.dew_point,"C");
  
  json_query("/current/clouds");
  cbm_read(LFN,wd.clouds,sizeof(wd.clouds));
  strcat(wd.clouds,"%");

  memset(val,0,sizeof(val));
  json_query("/current/visibility");
  cbm_read(LFN,val,sizeof(val));
  i = atoi(val);
  i /= 1000;
  itoa(i,wd.visibility,10);  
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
  clrscr();
  cbm_k_bsout('\x92');
  weather_get();

  y=3;
  x=17;

  // Date/time
  gotoxy(1,0);
  print(wd.date_txt);
  print("     ");
  print(wd.time_txt);

  // city
  gotoxy(1,y-2);
  print(l.city);
  print("\n ");
  print(l.region_code);
  print(" ");
  print(l.country_code);
  print("\n\n ");
  print(wd.description);
  gotoxy(0,y+4);

  // Icon
  weather_icon(wd.icon);
  gotoxy(x-7,y);
  print(wd.feels_like);

  print("\n\n\n\n");
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

  wait=65535U;

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
