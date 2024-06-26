/**
 * Weather / weather.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "constants.h"
#include "weather.h"
#include "options.h"
#include "location.h"
#include "screen.h"
#include "io.h"
#include "sprite.h"
#include "direction.h"
#include "ftime.h"
#include "input.h"
#include "utils.h"
#include "state.h"

extern OptionsData optData;
extern Location locData;

unsigned long dt, sunrise, sunset;
unsigned short timer = 65535;
bool forceRefresh = true;

char date_txt[32];
char sunrise_txt[16];
char sunset_txt[16];
char time_txt[16];
char feels_like[16];
unsigned short timezone_offset;
char pressure[14];
char humidity[16];
char dew_point[16];
char clouds[16];
char visibility[16];
char wind_speed[16];
char wind_dir[3];
char wind_txt[16];
char description[24];
char loc[48];
char timezone[48];
char temp[24];
unsigned char icon;

/*
  {"lat":33.1451,"lon":-97.088,"timezone":"America/Chicago","timezone_offset":-21600,"current":{"dt":1646341760,"sunrise":1646312048,"sunset":1646353604,"temp":25.42,"feels_like":24.7,"pressure":1018,"humidity":26,"dew_point":4.54,"uvi":3.66,"clouds":0,"visibility":10000,"wind_speed":5.14,"wind_deg":170,"weather":[{"id":800,"main":"Clear","description":"clear
  sky","icon":"01d"}]}}
 */

void weather_hpa_to_inhg(char *p)
{
    unsigned short w = atoi(p);

    w *= 3;
    itoa(w, p, 10);
    p[5] = p[4];
    p[4] = p[3];
    p[2] = '.';
}

void weather_date(char *c, unsigned long d, short offset)
{
    Timestamp ts;

    if (d != 0)
    {
        timestamp(d + offset, &ts);

        sprintf(c, "%u %s %u, %s", ts.day, time_month(ts.month), ts.year, time_dow(ts.dow));
    } else
        strcpy(c, "Unknown");
}

void weather_time(char *c, unsigned long d, short offset)
{
    Timestamp ts;

    if (d != 0)
    {
        timestamp(d + offset, &ts);

        sprintf(c, "%02u:%02u", ts.hour, ts.min);
    } else
        strcpy(c, "Unknown");
}
/*
weather_parse 
- Open the url containing the json data for the daily weather 

Returns
    true: Success
   false: Could not open the website, or could not find one of the json elements
 
*/
bool weather_parse(void)
{
    int success = 1;
    char units[14];
    char cc = 'C';
    unsigned char res;
    char failure[2] = {"?"};
    unsigned char tmp[128];
    char url[256];
    char *p;

    strcpy(units, "metric");
    if (optData.units == IMPERIAL)
        strcpy(units, "imperial");

    // if any json fails, then have default values
    dt = 0;
    sunrise = 0;
    sunset = 0;
    strcpy(temp, failure);
    strcpy(feels_like, failure);
    timezone_offset = 0;
    strcpy(pressure, failure);
    strcpy(humidity, failure);
    strcpy(dew_point, failure);
    strcpy(clouds, failure);
    strcpy(visibility, failure);
    strcpy(wind_speed, failure);
    strcpy(wind_dir, failure);
    strcpy(description, failure);
    icon = ICON_CLEAR_SKY;
   

    // http://api.openweathermap.org/data/2.5/onecall?lat=44.62335968017578&lon=-63.57278060913086&exclude=minutely,hourly,alerts,daily&units=%s&appid=2e8616654c548c26bc1c86b1615ef7f1

    snprintf(url, sizeof(url),  "N:HTTP://%s//data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts,daily&units=%s&appid=%s", 
                                OW_API, locData.latitude, locData.longitude, units,
                                OW_KEY);

    if (io_json_open(url))
    {
        return false;
    }

    // Grab the relevant bits
    if (io_json_query("/current/dt", tmp, sizeof(tmp)))
        return false;
    dt = atol(tmp);


    if (io_json_query("/timezone", tmp, sizeof(tmp)))
        return false;
    ellipsizeString( (char *) &tmp[0], &timezone[0], sizeof(timezone));


    if (io_json_query("/current/sunrise", tmp, sizeof(tmp)))
        return false;
    sunrise = atol(tmp);


    if (io_json_query("/current/sunset", tmp, sizeof(tmp)))
        return false;
    sunset = atol(tmp);


    if (io_json_query("/current/temp", tmp, sizeof(tmp)))
        return false;
    io_decimals(tmp,optData.maxPrecision);
    
    sprintf(temp, "%s*%c", tmp, optData.units == IMPERIAL ? 'F' : 'C');


    if (io_json_query("/current/feels_like", tmp, sizeof(tmp)))
        return false;
    io_decimals(tmp,optData.maxPrecision);
    sprintf(feels_like, "%s *%c", tmp, optData.units == IMPERIAL ? 'F' : 'C');


    if (io_json_query("/timezone_offset", tmp, sizeof(tmp)))
        return false;
    timezone_offset = atoi(tmp);


    if (io_json_query("/current/pressure", tmp, sizeof(tmp)))
        return false;

    if (optData.units == IMPERIAL)
        weather_hpa_to_inhg(tmp);

    sprintf(pressure, "%s %s", tmp, optData.units == IMPERIAL ? "\"Hg" : "mPa");


    if (io_json_query("/current/humidity", tmp, sizeof(tmp)))
        return false;
    sprintf(humidity, "%s%%", tmp);


    if (io_json_query("/current/dew_point", tmp, sizeof(tmp)))
        return false;
    sprintf(dew_point, "%s *%c", tmp, optData.units == IMPERIAL ? 'F' : 'C');


    if (io_json_query("/current/clouds", tmp, sizeof(tmp)))
        return false;
    sprintf(clouds, "%s%%", tmp);


    if (io_json_query("/current/visibility", tmp, sizeof(tmp)))
        return false;
    sprintf(visibility, "%d %s", atoi(tmp) / 1000, optData.units == IMPERIAL ? "mi" : "km");


    if (io_json_query("/current/wind_speed", tmp, sizeof(tmp)))
        return false;
    sprintf(wind_speed, "%s %s", tmp, optData.units == IMPERIAL ? "mph" : "kph");


    if (io_json_query("/current/wind_deg", tmp, sizeof(tmp)))
        return false;
    sprintf(wind_dir, "%s", degToDirection(atoi(tmp)));


    if (io_json_query("/current/weather/0/description", tmp, sizeof(tmp)))
        return false;
    sprintf(description, "%s", tmp);
    strcpy(description, strupr(description));


    if (io_json_query("/current/weather/0/icon", tmp, sizeof(tmp)))
        return false;
    icon = get_sprite(tmp);

    // Close connection
    io_json_close();

    return true;
}


void weather(void)
{
bool dayNight;
unsigned char bg, fg;
static  bool firstTime = true;    
static  FUJI_TIME future_time;
static  FUJI_TIME adjust_time;

    if (firstTime)
    {
        firstTime = false;
        io_time(&future_time);
    }

    if (wait_for_time(future_time))
    {
        forceRefresh = false;
        io_time(&future_time);
        memset(adjust_time, 0, sizeof(FUJI_TIME));

        adjust_time.minute = optData.refreshIntervalMinutes;
        add_time(future_time, future_time, adjust_time);

        screen_weather_parsing();

        if (!weather_parse())
            screen_weather_could_not_get();

        weather_date(date_txt,         dt, atoi(timezone_offset));
        weather_time(time_txt,         dt, timezone_offset);
        weather_time(sunrise_txt, sunrise, atoi(timezone_offset));
        weather_time(sunset_txt,   sunset, atoi(timezone_offset));

        sprintf(wind_txt, "%s %s", wind_speed, wind_dir);

        sprintf(loc, "%s, %s %s", locData.city, locData.region_code, locData.country_code);

        screen_colors(dt, timezone_offset, &fg, &bg, &dayNight);

        screen_daily(date_txt, icon, temp, pressure, description, loc, wind_txt, feels_like, 
                     dew_point, visibility, timezone, sunrise_txt, sunset_txt, humidity, clouds, 
                     time_txt, fg, bg, dayNight);
    }
    
    input_init();

    screen_weather_keys();

    timer = 65535;
    while (timer > 0)
    {
        if (input_weather() || forceRefresh)
            io_time(&future_time);

        csleep(1);

        if ((timer % CHECK_TIME_FREQUENCY) == 0)
            if (wait_for_time(future_time))
                timer = 1;

        timer--;
    }
}
