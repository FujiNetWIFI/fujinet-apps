/*
 * # Weather client for FujiNet                     
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <conio.h>

#include "fujinet-network.h"
//#include "hgrtext.h"
//#include "hgrtile.h"
#include "weatherdefs.h"
#include "weatherdisp.h"
#include "weatherinfo.h"

extern enum unit_option unit_opt;

void handle_err(char *message);

char url[256];
char  weather_head[] = "N:https://api.openweathermap.org/data/3.0/onecall?lat=";
char  direct_head[] = "N:http://api.openweathermap.org/geo/1.0/direct?q=";
char  reverse_head[] = "N:http://api.openweathermap.org/geo/1.0/reverse?lat=";
char url_lon[] = "&lon=";
char url_exclude[] = "&exclude=minutely,hourly,alerts";
char appkey_head[] = "&appid=";
char api_units[] = "&units=";
char ip_url[] = "N:http://ip-api.com/json/?fields=status,city,lon,lat";
char *unit_str[] = {"metric", "imperial"};

uint8_t appkey[33];

char city[40];
int	 err;

//
// change city name to upper case
//
char* toupper_str(char* s)
{
	char *p;
    for (p = s; *p != '\0'; ++p) {
        *p = toupper(*p);
    }
    return s;
}
//
// cut country code
//
char* cut_cc(char* s)
{
	char *p;
    for (p = s; *p != '\0'; ++p) {
		if (*p == ',') {
			*p = '\0';
		}
    }
    return s;
}
//
// setup OpenWeatherMap One Call URL
//
void setup_ocurl(LOCATION *loc) {
	strcpy(url, weather_head);
    strcat(url, loc->lat);
    strcat(url, url_lon);
    strcat(url, loc->lon);
    strcat(url, url_exclude);
    strcat(url, appkey_head);
    strcat(url, (char *)appkey);
    strcat(url, api_units);
	strcat(url, unit_str[unit_opt]);
}
//
// build direct geocall url
//
void setup_directurl(char *city) {
    strcpy(url, direct_head);
    strcat(url, city);
    strcat(url, appkey_head);
    strcat(url, (char *)appkey);
}
//
// build reverse geocall url
//
void setup_reverseurl(LOCATION *loc) {
    strcpy(url, reverse_head);
    strcat(url, loc->lat);
    strcat(url, url_lon);
    strcat(url, loc->lon);
    strcat(url, appkey_head);
    strcat(url, (char *)appkey);
}

//
// get location of city
//

bool direct_geocoding(LOCATION *loc, char *city) {
//	char param_str[40];
//	char query_str[40];
//	int ret;

	setup_directurl(city);

    network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(url);
    handle_err("direct parse");


    err = network_json_query(url, "/0/name", loc->city);
    handle_err("name query");
	if (strlen(loc->city) == 0) {
		return(false);
	}
/*
	strcpy(param_str, city);
	cut_cc(param_str);
	toupper_str(param_str);
	strcpy(query_str, loc.city);
	toupper_str(query_str);
	ret = strcmp(param_str, query_str);
	if (ret) {
		return(false);
	}
*/
/*
    err = network_json_query(url, "/0/country", loc->country);
    handle_err("name query");
*/

    err = network_json_query(url, "/0/lon", loc->lon);
    handle_err("name query");

    err = network_json_query(url, "/0/lat", loc->lat);
    handle_err("name query");

	network_close(url);

	return(true);
}

//
// get location info from ip 
//
bool get_location(LOCATION *loc) {
	char	buf[10];

	network_open(ip_url, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(ip_url);
    handle_err("parse");

	err = network_json_query(ip_url, "/status", buf);
    handle_err("status query");
	if (strcmp(buf, "success") != 0) {
		network_close(ip_url);
		return (false);
	}

	err = network_json_query(ip_url, "/city", city);
    handle_err("city query");

	err = network_json_query(ip_url, "/lon", loc->lon);
    handle_err("lon query");

	err = network_json_query(ip_url, "/lat", loc->lat);
    handle_err("lat query");

	network_close(ip_url);

	return(true);
}
//
// extract time string 'hh:mm'
//
char *time_str(char *buf) {
	*(buf + 16) = 0x00;
	return((buf + 11));
}

//
// get city name from lon, lat
//
void reverse_geocoding(LOCATION *l, WEATHER *w) {

	setup_reverseurl(l);

    network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(url);
    handle_err("parse");

    err = network_json_query(url, "/0/name", w->name);
    handle_err("name query");

    err = network_json_query(url, "/0/country", w->country);
    handle_err("country query");

	network_close(url);
}
//
//
//
void get_owm_info(LOCATION *loc, WEATHER *wi, FORECAST *fc) {
	char querybuf[40];

	reverse_geocoding(loc, wi);

	setup_ocurl(loc);
    network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(url);
    handle_err("parse");


//  date & time
    err = network_json_query(url, "/current/dt", querybuf);
    handle_err("dt query");
	wi->td = atol(querybuf);
//  timezone(offset) 
    err = network_json_query(url, "/timezone_offset", querybuf);
    handle_err("tz query");
	wi->tz = atol(querybuf);
// timezone
    err = network_json_query(url, "/timezone", wi->timezone);
    handle_err("timezone query");
// description
    err = network_json_query(url, "/current/weather/0/description", wi->description);
    handle_err("description query");
// icon
    err = network_json_query(url, "/current/weather/0/icon", wi->icon);
    handle_err("icon query");

//	sunrise
    err = network_json_query(url, "/current/sunrise", querybuf);
    handle_err("sunrise query");
	wi->sunrise = atol(querybuf);
//  sunset
    err = network_json_query(url, "/current/sunset", querybuf);
    handle_err("sunset query");
	wi->sunset = atol(querybuf);
//  temperature
    err = network_json_query(url, "/current/temp", wi->temp);
    handle_err("temp query");
//  feels_like
    err = network_json_query(url, "/current/feels_like", wi->feels_like);
    handle_err("feels-like query");
//  pressure
    err = network_json_query(url, "/current/pressure", wi->pressure);
    handle_err("feels-like query");
//  humidity
    err = network_json_query(url, "/current/humidity", wi->humidity);
    handle_err("humidity query");
//  dew_point
    err = network_json_query(url, "/current/dew_point", wi->dew_point);
    handle_err("dew-point query");
//  visibility
    err = network_json_query(url, "/current/visibility", wi->visibility);
    handle_err("visibility query");
//  wind_speed
    err = network_json_query(url, "/current/wind_speed", wi->wind_speed);
    handle_err("wind_speed query");
//  wind_deg
    err = network_json_query(url, "/current/wind_deg", wi->wind_deg);
    handle_err("wind_deg query");
//  clouds
    err = network_json_query(url, "/current/clouds", wi->clouds);
    handle_err("wind_deg query");
//	forecast
	set_forecast(fc);

	network_close(url);
}
//
// set forecast data
//
void set_forecast(FORECAST *fc) {
	char	i;
	char querybuf[40];
	char prbuf[40];

	for (i=0; i<=7; i++) {
// date & time
		sprintf(querybuf, "/daily/%d/dt", i);
	    err = network_json_query(url, querybuf, prbuf);
    	handle_err("fc dt  query");
		fc->day[i].td = atol(prbuf);
// sunrise 
		sprintf(querybuf, "/daily/%d/sunrise", i);
	    err = network_json_query(url, querybuf, prbuf);
    	handle_err("fc sunrise  query");
		fc->day[i].sunrise = atol(prbuf);
// sunset 
		sprintf(querybuf, "/daily/%d/sunset", i);
	    err = network_json_query(url, querybuf, prbuf);
    	handle_err("fc sunrise  query");
		fc->day[i].sunset = atol(prbuf);
// temp day
		sprintf(querybuf, "/daily/%d/temp/day", i);
	    err = network_json_query(url, querybuf, fc->day[i].temp_day);
    	handle_err("fc temp  query");
// temp min
		sprintf(querybuf, "/daily/%d/temp/min", i);
	    err = network_json_query(url, querybuf, fc->day[i].temp_min);
    	handle_err("fc temp min  query");
// temp max
		sprintf(querybuf, "/daily/%d/temp/max", i);
	    err = network_json_query(url, querybuf, fc->day[i].temp_max);
    	handle_err("fc temp max  query");
// description
		sprintf(querybuf, "/daily/%d/weather/0/description", i);
	    err = network_json_query(url, querybuf, fc->day[i].description);
    	handle_err("fc description  query");
// icon
		sprintf(querybuf, "/daily/%d/weather/0/icon", i);
	    err = network_json_query(url, querybuf, fc->day[i].icon);
    	handle_err("fc icon  query");
// pressure
		sprintf(querybuf, "/daily/%d/pressure", i);
	    err = network_json_query(url, querybuf, fc->day[i].pressure);
    	handle_err("fc icon  query");
// dew point
		sprintf(querybuf, "/daily/%d/dew_point", i);
	    err = network_json_query(url, querybuf, fc->day[i].dew_point);
    	handle_err("fc dew point  query");
// wind  speed
		sprintf(querybuf, "/daily/%d/wind_speed", i);
	    err = network_json_query(url, querybuf, fc->day[i].wind_speed);
    	handle_err("fc wind speed  query");
// wind  deg
		sprintf(querybuf, "/daily/%d/wind_deg", i);
	    err = network_json_query(url, querybuf, fc->day[i].wind_deg);
    	handle_err("fc wind deg  query");
// humidity 
		sprintf(querybuf, "/daily/%d/humidity", i);
	    err = network_json_query(url, querybuf, fc->day[i].humidity);
    	handle_err("fc humidity query");
	}
}
//
void handle_err(char *message) {
	if (err <  0) {
		printf("err: %s\n", message);
		exit(1);
	}
}
