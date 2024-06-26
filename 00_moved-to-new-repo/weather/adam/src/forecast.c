/**
 * Weather / forecast.c
s *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "constants.h"
#include "options.h"
#include "location.h"
#include "screen.h"
#include "io.h"
#include "sprite.h"
#include "direction.h"
#include "sprite.h"
#include "ftime.h"
#include "input.h"
#include "forecast.h"
#include "screen.h"
#include "utils.h"

ForecastData forecastData;
unsigned char forecast_offset = 0;

extern unsigned short timer;
extern bool forceRefresh;

extern OptionsData optData;
extern Location locData;



/*** FORECAST ***/

// HTTP://api.openweathermap.org/data/2.5/onecall?lat=44.62335968017578&lon=-63.57278060913086&exclude=minutely,hourly,alerts,current&units=%s&appid=2e8616654c548c26bc1c86b1615ef7f1

/*
forecast_open 
- Open the url containing the json data for the forecastd 

Returns
    true: Success
   false: Could not open the website
 
*/
bool forecast_open(void)
{
    Timestamp ts;
    char url[256];
    char units[10];

    if (optData.units == METRIC)
        strcpy(units, "metric");
    else if (optData.units == IMPERIAL)
        strcpy(units, "imperial");

    snprintf(url, sizeof(url), "N:HTTP://%s//data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts,current&units=%s&appid=%s", 
                OW_API, locData.latitude, locData.longitude, units,
                OW_KEY);

    if (io_json_open(url) == 0)
        return false;
    else
        return true;
}


bool forecast_close(void)
{

    io_json_close();

    return true;
}

/*
Collect the i-th forecast and parse the collected json 
and storing the information in the ForecastData structure
*/

void forecast_parse(unsigned char i, ForecastData *f)
{
  Timestamp ts;
  char prefix[20];
  char request[60];
  char json_part[30];


  snprintf(prefix, sizeof(prefix), "/daily/%c/", '0'+i);

  snprintf(request, sizeof(request), "%sdt", prefix);
  io_json_query(request, json_part, sizeof(json_part));
  timestamp(atol(json_part),&ts);

  snprintf(f->date,8,"%d %s",ts.day,time_month(ts.month));
  snprintf(f->dow,4,"%s",time_dow(ts.dow));

  snprintf(request, sizeof(request), "%sweather/0/description", prefix);
  io_json_query(request, f->desc, sizeof(f->desc));

  snprintf(request, sizeof(request), "%sweather/0/icon", prefix);
  io_json_query(request, json_part, sizeof(json_part));
  f->icon=get_sprite(json_part);

  snprintf(request, sizeof(request), "%stemp/min", prefix);
  io_json_query(request, f->lo, sizeof(f->lo));
  io_decimals(f->lo, optData.maxPrecision);

  snprintf(request, sizeof(request), "%stemp/max", prefix);
  io_json_query(request, f->hi, sizeof(f->hi));
  io_decimals(f->hi, optData.maxPrecision);

  snprintf(request, sizeof(request), "%spressure", prefix);
  io_json_query(request, f->pressure, sizeof(f->pressure));

  snprintf(request, sizeof(request), "%swind_speed", prefix);
  io_json_query(request, json_part, sizeof(json_part));
  io_decimals(json_part, optData.maxPrecision);
  snprintf(f->wind, sizeof(f->wind), "WIND:%s %s ",json_part,optData.units == IMPERIAL ? "mph" : "kph");

  snprintf(request,  sizeof(request),"%swind_deg", prefix);
  io_json_query(request, json_part, sizeof(json_part));
  strcat(f->wind,degToDirection(atoi(json_part)));

  snprintf(request,  sizeof(request),"%spop", prefix);
  io_json_query(request, f->pop, sizeof(f->pop));

  snprintf(request,  sizeof(request),"%srain", prefix);
  io_json_query(request, f->rain, sizeof(f->rain));

  //snprintf(request,  sizeof(request),"%ssnow", prefix);
  //io_json_query(request, f->snow, sizeof(f->snow));
  strcpy(f->snow, "");

}

void forecast(void)
{
  unsigned char bg, fg;
  bool dayNight;
static  bool firstTime = true;    
static  FUJI_TIME future_time;
static  FUJI_TIME adjust_time;
  

  if (firstTime)
  {
      firstTime = false;
      io_time(&future_time);
  }

  if (wait_for_time(future_time) || forceRefresh)
  {
    forceRefresh = false;

    io_time(&future_time);
    memset(adjust_time, 0, sizeof(FUJI_TIME));

    adjust_time.minute = optData.refreshIntervalMinutes;
    add_time(future_time, future_time, adjust_time);

    screen_forecast_init();
    
    screen_colors(dt, timezone_offset, &fg, &bg, &dayNight);
    
    if (forecast_open())
      screen_weather_could_not_get();
    else
    { 
      clear_all_sprites();  
      for (int i=0;i<4;i++)
      {
        forecast_parse((unsigned char) (i + forecast_offset), &forecastData);
        screen_forecast(i,&forecastData, fg, bg, true);
      }
      display_sprites();
      forecast_close();
      
    }
  }


  
  input_init();

  screen_forecast_keys();

  timer = 65535;
  while (timer > 0)
  {
    if (input_forecast() || forceRefresh)
      io_time(&future_time);

    csleep(1);

    if ((timer % CHECK_TIME_FREQUENCY) == 0)
      if (wait_for_time(future_time))
        timer = 1;

    timer--;
   }

}
