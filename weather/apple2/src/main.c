/*
 * # Wolrd Weather from FujiNet                     
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "weatherdefs.h"
#include "weatherui.h"
#include "weatherinfo.h"
#include "weatherdisp.h"
#include "appkey.h"

LOCATION loc;
LOCATION current;
WEATHER wi;
FORECAST fc;
char	current_screen;

typedef enum command {
	COM_REFRESH,
	COM_WEATHER,
	COM_FORECAST,
	COM_NONE
} COMMAND;

enum unit_option unit_opt = METRIC;

void main(void)
{
	bool	quit = false;
	char	forecast_page;
	char ch;
	COMMAND com = COM_REFRESH;

	get_appkey();

	disp_message("     fetching location data...");
	if (!get_location(&loc)) {
		disp_message("Sorry, Internal Error. Exit\n");
		exit(1);
	}
	current = loc;
	forecast_page = 0;

	while (!quit) {
		switch (com) {
			case COM_REFRESH:		// refresh
				disp_message("     fetching weather data...");
				get_owm_info(&loc, &wi, &fc);
				current_screen = 0;
			case COM_WEATHER:		// weather
				disp_weather(&wi);
				disp_menu("F)orecast R)efresh U)nit L)ocation Q)uit");
				break;
			case COM_FORECAST:		// forecast
				disp_forecast(&fc, forecast_page);
				disp_menu(" N)ext  B)ack  W)eather                 ");
				break;
			default:
				;
		}
		ch = get_keyin();
		if (current_screen == 1) {			// weather
			switch (ch) {
				case 'r':
				case 'R':
					com = COM_REFRESH;
					break;
				case 'u':
				case 'U':
					unit_opt = (unit_opt ==  METRIC) ? IMPERIAL : METRIC;
					com = COM_REFRESH;
					break;
				case 'l':
				case 'L':
					change_location(&loc);
					com = COM_REFRESH;
					break;
				case 'q':
				case 'Q':
					quit = true;
					break;
				case 'f':
				case 'F':
					com = COM_FORECAST;
					forecast_page = 1;
					break;
				default:
					com = COM_NONE;
			}
		}
		else {								// forecast
			switch (ch) {
			case 'w':
			case 'W':
				com = COM_WEATHER;
				break;
			case 'n':
			case 'N':
				com = COM_FORECAST;
				forecast_page = 2;
				break;
			case 'b':
			case 'B':
				com = COM_FORECAST;
				forecast_page = 1;
				break;
			default:
				com = COM_NONE;
			}
		}
	}
	clear_display();
	exit(0);
}
