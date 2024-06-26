/**
 * Weather
 *
 * Screen routines
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include "forecast.h"
#include "location.h"
#include "options.h"

#define CLIENT_VERSION "1.0"
void screen_init(void);

#define STATUS_DELAY (100)
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
void screen_options_load_failed(void);
void screen_options_could_not_save(void);
void screen_options_success(void);
void screen_options_saving(void);
bool screen_options(OptionsData *o);

void screen_icon_reset(void); 
void screen_icon(unsigned char x, unsigned char y, unsigned char icon, bool dayNight);

bool screen_location(Location *l, bool *autoip, bool *manual);
void screen_location_load_failed(void);
void screen_location_ip_detect(void);
void screen_location_could_not_ip_detect(void);
void screen_location_could_not_detect(void);
void screen_location_position_detect(void);
void screen_location_success(void);
void screen_location_could_not_save(void);
void screen_location_saving(void);
void screen_location_load_defaults(void);
void screen_current_time(void);
void screen_location_position_found(void);
void screen_location_saved(void);


void screen_weather_init(void);
void screen_weather_could_not_get(void);
void screen_weather_parsing(void);
void screen_weather_keys(void);

void screen_forecast(unsigned char i, ForecastData *f, unsigned char foregroundColor, unsigned char backgroundColor, bool day);

void screen_forecast_keys(void);

#endif /* SCREEN_H */
