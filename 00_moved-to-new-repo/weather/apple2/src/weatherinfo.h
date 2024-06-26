#ifndef	WEATHERINFO_H
#define WEATHERINFO_H
#include <stdbool.h>

// function
bool get_location(LOCATION *loc);

// openweathermap
void setup_owmurl();
char *time_str(char *buf);
void reverse_geocoding(LOCATION *loc, WEATHER *wi);
bool direct_geocoding(LOCATION *loc, char *city);
void get_owm_info(LOCATION *loc, WEATHER *wi, FORECAST *fc);
void set_forecast(FORECAST *fc);

void handle_err(char *message);
#endif
