#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	<conio.h>
#include	"hgrtext.h"
#include	"weatherdefs.h"
#include	"weatherinfo.h"
#include	"weatherdisp.h"

#define FUJICMD_READ_APP_KEY 0xdd

extern char disp_page;
extern char current_screen;
extern enum unit_option unit_opt;

char c_str[] = {0x22, 0x43, 0x00};
char f_str[] = {0x22, 0x46,0x00};
char min_str[] = {0x23, 0x24,0x00};
char max_str[] = {0x26, 0x27,0x00};

char *temp_unit[] = {c_str, f_str};
char *speed_unit[] = {" m/s", "mph"};

char *wind_deg[] = {" N", " NE", " E", " SE", " S", " SW", " W", " NW"};

char current_forecast_page = 0;

char icon_code(char *buf) {
	int	value;
	char code;

	*(buf + 2) = 0x00;
	value = atoi(buf);
	switch (value) {
		case	1:
		case	2:
		case	3:
		case	4:
			code = value - 1;
			break;
		case	9:
			code = 4;
			break;
		case	10:
			code = 5;
			break;
		case	11:
			code = 6;
			break;
		case	13:
			code = 7;
			break;
		case	50:
			code = 8;
			break;
	}
	return (code);
}

// Hiresページ1のメモリアドレス
#define HIRES_PAGE1 0x2000
#define HIRES_SIZE  0x2000

void clear_hires_page1() {
    memset((void*)HIRES_PAGE1, 0, HIRES_SIZE);
}


void draw_weather_icon(char *buf, char col, char row) {
	set_colrow(col, row);
	draw_tile(icon_code(buf));
}

// draw sunrise / sunset icon
void draw_sunrs() {
	set_colrow(26, 11);
	draw_tile(9);
}

char padding_center(char *s) {
	char len;

	len = strlen(s);
	return((40 - len) / 2);
}
//
// main display function
//
void disp_weather(WEATHER *wi) {
	char row;
	char time_buf[40];
	char prbuf[40];
	long localtime;
	long visi;
	char wind_idx;

	if (current_screen == 1) {
		return;
	}
	current_screen = 1;
	current_forecast_page = 0;

	init_hgr();
	clear_hires_page1();
//

// line 8 name
	sprintf(prbuf, "%s, %s", wi->name, wi->country); 
	set_colrow(padding_center(prbuf), 8);
	draw_string(prbuf);

//  line 0 datetime
	localtime = wi->td + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	prbuf[strlen(prbuf)-1] = 0x00;
	set_colrow(padding_center(prbuf), 0);
	draw_string(prbuf);

//  line 2  temperature
	row = 2;
	sprintf(prbuf, "%s%s", wi->temp, temp_unit[unit_opt]);
	set_colrow(padding_center(prbuf), row);
    draw_string(prbuf);

//  line 4 description
	set_colrow(12,4);
	draw_string(wi->description);

//  line 5 pressure
	set_colrow(12,5);
	sprintf(prbuf, "%s%s", wi->pressure, "hPa");
	draw_string(prbuf);

//  line j icon
	draw_weather_icon(wi->icon, 6, 4);

//  line 11,12 sunrise/set icon
	draw_sunrs();

//	line 11 sunrise
	localtime = wi->sunrise + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	strcpy(time_buf, time_str(prbuf));
	set_colrow(32,11);
	draw_string(time_buf);

//  line 12 runset
	localtime = wi->sunset + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	strcpy(time_buf, time_str(prbuf));
	set_colrow(32,12);
	draw_string(time_buf);


//  line 11 feels like
	row = 11;
    set_row(row);
	sprintf(prbuf, "Feels like: %s%s", wi->feels_like, temp_unit[unit_opt]);
    draw_string(prbuf);

//  line 12 wind speed, deg
	row = 12;
    set_row(row);
	wind_idx = atoi(wi->wind_deg) / 45;
	sprintf(prbuf, "Wind Speed: %s%s%s", wi->wind_speed, speed_unit[unit_opt], wind_deg[wind_idx]);
    draw_string(prbuf);

//  line 14 dew point
	row = 14;
	set_row(row);
	sprintf(prbuf, "Dew Point : %s%s", wi->dew_point, temp_unit[unit_opt]);
	draw_string(prbuf);

//  line 14 humidity
	set_colrow(24, row);
	sprintf(prbuf, "Humidity: %s%%", wi->humidity);
	draw_string(prbuf);

//  line 15 visibility
	row = 15;
	set_row(row);
	strcpy(prbuf, wi->visibility);
	visi = atol(prbuf) / 1000;
	sprintf(prbuf, "Visibility: %ld km", visi);
	draw_string(prbuf);

//  line 15 clouds
	set_colrow(24, row);
	sprintf(prbuf, "Clouds  : %s%%", wi->clouds);
	draw_string(prbuf);

//  line 18 timezone
	row = 18;
	set_colrow(2, row);
	sprintf(prbuf, "TimeZone: %s", wi->timezone);
	draw_string(prbuf);
}

//
//  display forecast screen
//
void disp_forecast(FORECAST *fc, char p) {
	char	i;
	char	page;
	char    tdbuf[40];
	char	prbuf[10];
	long	localtime;
	char	wind_idx;

	if (p == current_forecast_page) {
		return;
	}
	current_screen = 2;
	current_forecast_page = p;
		
	page = (p-1) * 4;
	init_hgr();
	clear_hires_page1();

//	draw heder
	set_colrow(0, 10);
	draw_string(min_str);
	set_colrow(0, 9);
	draw_string(max_str);
	set_colrow(0, 16);
	draw_string("Hum: ");

	for (i=0; i<=3; i++) {
//   day         
    	localtime = fc->day[i+page].td;
		strcpy(tdbuf, ctime((const unsigned long *)&localtime));
    	strncpy(prbuf, (char *)&tdbuf[8], 2);
		prbuf[2] = 0x00;
		set_colrow((i*10)+5, 1);
		draw_string(prbuf);
		
//   month
    	strncpy(prbuf, (char *)&tdbuf[4], 3);
		prbuf[3] = 0x00;
		set_colrow((i*10)+5, 2);
		draw_string(prbuf);

//   weekday
    	strncpy(prbuf, (char *)&tdbuf[0], 3);
		set_colrow((i*10)+5, 7);
		draw_string(prbuf);

//   weather icon
		draw_weather_icon(fc->day[i+page].icon, (i*10)+4, 4);

//   max temp
		sprintf(prbuf, "%s%s", fc->day[i+page].temp_max, temp_unit[unit_opt]);
		set_colrow((i*10)+3, 9);
		draw_string(prbuf);

//   min temp
		sprintf(prbuf, "%s%s", fc->day[i+page].temp_min, temp_unit[unit_opt]);
		set_colrow((i*10)+3, 10);
		draw_string(prbuf);

//   pressure
		sprintf(prbuf, "%shPa", fc->day[i+page].pressure);
		set_colrow((i*10)+3, 12);
		draw_string(prbuf);

//   wind degree
    	wind_idx = atoi(fc->day[i+page].wind_deg) / 45;
		sprintf(prbuf, "Wind:%s", wind_deg[i+page]);
		set_colrow((i*10)+2, 13);
		draw_string(prbuf);

//   wind speed
		sprintf(prbuf, "%s%s", fc->day[i+page].wind_speed, speed_unit[unit_opt]);
		set_colrow((i*10)+2, 14);
		draw_string(prbuf);
//   humidity
		sprintf(prbuf, "%s%%", fc->day[i+page].humidity);
		set_colrow((i*10)+5, 16);
		draw_string(prbuf);

	}
}
