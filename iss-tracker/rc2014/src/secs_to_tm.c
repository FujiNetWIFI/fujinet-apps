#include "time_impl.h"
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

void secs_to_tm(uint32_t seconds, struct tm *tm)
{
	uint32_t minutes, hours, days, year, month;
	uint32_t dayOfWeek;

	/* calculate minutes */
	minutes  = seconds / 60;
	seconds -= minutes * 60;
	/* calculate hours */
	hours    = minutes / 60;
	minutes -= hours   * 60;
	/* calculate days */
	days     = hours   / 24;
	hours   -= days    * 24;

	/* Unix time starts in 1970 on a Thursday */
	year      = 1970;
	dayOfWeek = 4;

	while(1) {
		bool     leapYear   = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		uint16_t daysInYear = leapYear ? 366 : 365;
		if (days >= daysInYear) {
			dayOfWeek += leapYear ? 2 : 1;
			days      -= daysInYear;
			if (dayOfWeek >= 7)
				dayOfWeek -= 7;
			++year;
		} else {
			tm->tm_yday = days;
			dayOfWeek  += days;
			dayOfWeek  %= 7;

			/* calculate the month and day */
			static const uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
			for(month = 0; month < 12; ++month) {
				uint8_t dim = daysInMonth[month];

				/* add a day to feburary if this is a leap year */
				if (month == 1 && leapYear)
					++dim;

				if (days >= dim)
					days -= dim;
				else
					break;
			}
			break;
		}
	}

	tm->tm_sec  = seconds;
	tm->tm_min  = minutes;
	tm->tm_hour = hours;
	tm->tm_mday = days + 1;
	tm->tm_mon  = month;
	tm->tm_year = year-1900;
	tm->tm_wday = dayOfWeek;
}

