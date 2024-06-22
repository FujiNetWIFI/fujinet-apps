#ifndef	WEATHERDEFS_H
#define WEATHERDEFS_H
#define	LINE_LEN	40
#define	HALF_LEN	20
#define	QUARTER_LEN	10
struct location_info {
	char	lon[HALF_LEN];
	char	lat[HALF_LEN];
	char	city[HALF_LEN];
};
typedef struct location_info LOCATION;

struct weather_info {
	char	name[LINE_LEN];
	char	country[LINE_LEN];
	char	timezone[LINE_LEN];
	long	td;
	long	tz;						// time-zone offset
	char	description[LINE_LEN];
	char	icon[5];
	long	sunrise;	
	long	sunset;
	char	temp[QUARTER_LEN];	
	char	feels_like[QUARTER_LEN];
	char	pressure[QUARTER_LEN];
	char	humidity[QUARTER_LEN];
	char	dew_point[QUARTER_LEN];
//	char	uvi[QUARTER_LEN];
	char	clouds[QUARTER_LEN];
	char	visibility[QUARTER_LEN];
	char	wind_speed[QUARTER_LEN];
	char	wind_deg[QUARTER_LEN];
//	char	wind_gust[QUARTER_LEN];
};
typedef struct weather_info WEATHER;

struct day_info {
	long	td;
	long	sunrise;	
	long	sunset;
	char	temp_day[QUARTER_LEN]; //daily/n/temp/day
	char	temp_min[QUARTER_LEN];
	char	temp_max[QUARTER_LEN];
	char	description[LINE_LEN]; //daily/n/weather/n/description
	char	icon[5];   				// /daily/n/weather/n/icon
	char	pressure[QUARTER_LEN]; //daily/n/pressure
	char	dew_point[QUARTER_LEN]; //daily/n/dew_point
	char	wind_speed[QUARTER_LEN]; //daily/n/wind_speed
	char	wind_deg[QUARTER_LEN]; //daily/n/wind_deg
	char	humidity[QUARTER_LEN]; //daily/n/humidity
};
typedef struct day_info  DAYINFO;

struct forecast_info {
	DAYINFO day[8];
};
typedef struct forecast_info  FORECAST;
enum unit_option {
    METRIC,
    IMPERIAL
};
#endif
