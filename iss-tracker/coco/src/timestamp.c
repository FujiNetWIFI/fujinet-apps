/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose UNIX Timestamp to string function
 */

#include <cmoc.h>
#include <coco.h>

#define true 1
#define false 0

/**
 * @brief Days of month for each month
 * @verbose leap year is taken care of in timestamp()
 */
const unsigned char daysOfMonth[12] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

/**
 * @brief month value to text
 * @param mon Month value (0-11)
 * @return Three letter + null month string.
 */
const char *tm_mon(int mon)
{
    switch(mon)
    {
    case 0:
        return "JAN";
    case 1:
        return "FEB";
    case 2:
        return "MAR";
    case 3:
        return "APR";
    case 4:
        return "MAY";
    case 5:
        return "JUN";
    case 6:
        return "JUL";
    case 7:
        return "AUG";
    case 8:
        return "SEP";
    case 9:
        return "OCT";
    case 10:
        return "NOV";
    case 11:
        return "DEC";
    }
}

/**
 * @brief write human readable date/time from unix timestam p
 * @param seconds The UNIX timestamp (64-bit)
 * @param tso The output area for timestamp (12 chars needed) 
 */
void timestamp(long seconds, char *_tso)
{
    int extraDays;
    int index;
    int date, month, hours, minutes, secondss;
    long currYear, daysTillNow, extraTime;
    int flag = 0;

    daysTillNow = seconds / (24 * 60 * 60);
    extraTime = seconds % (24 * 60 * 60);
    currYear = 1970;

        // Calculating current year
    while (true) {
        if (currYear % 400 == 0
            || (currYear % 4 == 0 && currYear % 100 != 0)) {
            if (daysTillNow < 366) {
                break;
            }
            daysTillNow -= 366;
        }
        else {
            if (daysTillNow < 365) {
                break;
            }
            daysTillNow -= 365;
        }
        currYear += 1;
    }

    // Updating extradays because it
    // will give days till previous day
    // and we have include current day
    extraDays = daysTillNow + 1;
 
    if (currYear % 400 == 0
        || (currYear % 4 == 0 && currYear % 100 != 0))
        flag = 1;

    // Calculating MONTH and DATE
    month = 0, index = 0;
    if (flag == 1) {
        while (true) {
 
            if (index == 1) {
                if (extraDays - 29 < 0)
                    break;
                month += 1;
                extraDays -= 29;
            }
            else {
                if (extraDays - daysOfMonth[index] < 0) {
                    break;
                }
                month += 1;
                extraDays -= daysOfMonth[index];
            }
            index += 1;
        }
    }
    else {
        while (true) {
 
            if (extraDays - daysOfMonth[index] < 0) {
                break;
            }
            month += 1;
            extraDays -= daysOfMonth[index];
            index += 1;
        }
    }

    // Current Month
    if (extraDays > 0) {
        month += 1;
        date = extraDays;
    }
    else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = daysOfMonth[month - 1];
        }
    }

    // Calculating HH:MM:YYYY
    hours = extraTime / 3600;
    minutes = (extraTime % 3600) / 60;
    secondss = (extraTime % 3600) % 60;

    // Write output to target string.
    sprintf(_tso,"  %s %02u %02u:%02u",tm_mon(month),date,hours,minutes);
}
