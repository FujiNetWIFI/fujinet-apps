/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose On Screen Display Functions
 */

#include <cmoc.h>
#include "globals.h"
#include "pmode3.h"
#include "ftime.h"

char lon_output[20];
char lat_output[20];
char ts_output[20];

/**
 * @brief Update output strings
 */
void _update_output(void)
{
    Timestamp tm;
    
    timestamp(ts,&tm);
    sprintf(lon_output,"%5s %9s","LON:",lon_s);
    sprintf(lat_output,"%5s %9s","LAT:",lat_s);
    sprintf(ts_output,"  %s %02u %02u:%02u",time_month(tm.month),tm.day,tm.hour,tm.min);
}

/**
 * @brief output on screen display
 */
void osd(void)
{
    _update_output();
    puts(0,160,0,"  ISS POSITION  ");
    puts(0,168,2,lon_output);
    puts(0,176,2,lat_output);
    puts(0,184,0,ts_output);
}
