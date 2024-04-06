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
#include "timestamp.h"

char lon_output[17];
char lat_output[17];

/**
 * @brief Update output strings
 */
void _update_output(void)
{
    sprintf(lon_output,"%5s %11s","LON: ",lon_s);
    sprintf(lat_output,"%5s %11s","LAT: ",lat_s);
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
}
