/**
 * Weather / input.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <eos.h>
#include <stdbool.h>
#include "input.h"
#include "constants.h"
#include "options.h"
#include "location.h"
#include "state.h"
#include "forecast.h"
#include "screen.h"

extern unsigned short timer;
extern bool forceRefresh;


void input_init(void) { 
    eos_start_read_keyboard(); 
}

bool input_weather(void)
{
    switch (eos_end_read_keyboard())
    {
    case 0x81: // F1 OPTIONS
        if (screen_options(&optData))
            options_save(&optData);
        timer = 1;
        break;

    case 0x83: // F3 LOCATION
        state = LOCATION;
        locData.forceManualLocation = true;
        timer = 1;
        break;

    case 0x84: // F4 FORECAST
        state = FORECAST;
        timer = 1;
        break;

    case 0x85: // F5 UNITS
        optData.units = (optData.units == IMPERIAL ? METRIC : IMPERIAL);
        options_save(&optData);
        timer = 1;
        break;
        
    case 0x86: // F6  REFRESH
        timer = 1;
        break;
    }

    return (bool) (timer == 1);
}

bool input_forecast(void)
{
    switch (eos_end_read_keyboard())
    {
    case 0x81: // F1 OPTIONS
        if (screen_options(&optData))
            options_save(&optData);
        timer = 1;
        break;

    case 0x82: // F2 NEXT/PREVIOUS PAGE
        if (forecast_offset == 0)
            forecast_offset = 4;
        else
            forecast_offset = 0;
        timer = 1;
        break;

    case 0x83: // F3 LOCATION
        state = LOCATION;
        locData.forceManualLocation = true;
        timer = 1;
        break;

    case 0x84: // F4 WEATHER
        state = WEATHER;
        timer = 1;
        break;

    case 0x85: //F5 UNITS
        optData.units = (optData.units == IMPERIAL ? METRIC : IMPERIAL);
        options_save(&optData);
        timer = 1;
        break;

    case 0x86: // F6 REFRESH
        timer = 1;
        break;
    }

    return (bool) (timer == 1);
}
