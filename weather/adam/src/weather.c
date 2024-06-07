/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "weather.h"
#include "options.h"
#include "location.h"
#include "screen.h"
#include "io.h"
#include "faux_json.h"
#include "direction.h"
#include "icon.h"
#include "ftime.h"
#include "input.h"

extern OptionsData optData;
extern Location locData;

unsigned long dt, sunrise, sunset;
unsigned short timer = 65535;

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
char timezone[24];
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

    timestamp(d + offset, &ts);

    sprintf(c, "%u %s %u, %s", ts.day, time_month(ts.month), ts.year, time_dow(ts.dow));
}

void weather_time(char *c, unsigned long d, short offset)
{
    Timestamp ts;

    timestamp(d + offset, &ts);

    sprintf(c, "%02u:%02u", ts.hour, ts.min);
}

void weather(void)
{
    bool dayNight;
    unsigned char bg, fg;

    timer = 65535;

    memset(json, 0, sizeof(json));
    screen_weather_parsing();

    if (!io_weather(json))
        screen_weather_could_not_get();

    weather_date(date_txt, dt, atoi(timezone_offset));
    weather_time(time_txt, dt, timezone_offset);
    weather_time(sunrise_txt, sunrise, atoi(timezone_offset));
    weather_time(sunset_txt, sunset, atoi(timezone_offset));

    sprintf(wind_txt, "%s %s", wind_speed, wind_dir);

    sprintf(loc, "%s, %s %s", locData.city, locData.region_code, locData.country_code);

    screen_colors(dt, timezone_offset, &fg, &bg, &dayNight);

    screen_daily(date_txt, icon, temp, pressure, description, loc, wind_txt, feels_like, dew_point, visibility, timezone, sunrise_txt, sunset_txt, humidity, clouds, time_txt, fg, bg, dayNight);

    input_init();

    while (timer > 0)
    {
        input_weather();
        csleep(1);
        timer--;
    }
}
