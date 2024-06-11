/**
 * Weather
 *
 * Screen routines
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include "forecast.h"

void screen_init(void);

/*
screen_colors 
- Sets the colours based on the time of day

Parameters
in
- d:                univeral time
- offset:           time zone offset

out
- fg:               foreground colour based on time
- bg:               background colour based on time
- dayNight:         true = day time
*/
void screen_colors(unsigned long d, short offset, unsigned char *fg, unsigned char *bg, bool *dayNight);

void screen_welcome(void);

void screen_daily(char *date, unsigned char icon, char *temperature, char *pressure, char *description, char *location, char *wind, char *feels, char *dew, char *visibility, char *timezone,
                  char *sunrise, char *sunset, char *humidity, char *clouds, char *time, unsigned char foregroundColor, unsigned char backgroundColor, bool dayNight);

void screen_options_init(void);
void screen_options_init_not_found(void);
void screen_options_could_not_save(void);

void screen_icon(unsigned char x, unsigned char y, unsigned char unused, unsigned char icon, bool dayNight);

void screen_location_detect(void);

void screen_weather_init(void);
void screen_weather_could_not_get(void);
void screen_weather_parsing(void);

void screen_forecast(unsigned char i, ForecastData *f, unsigned char foregroundColor, unsigned char backgroundColor, bool day);

void screen_forecast_keys(void);

#endif /* SCREEN_H */
