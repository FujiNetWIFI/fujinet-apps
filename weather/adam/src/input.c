/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <eos.h>
#include "input.h"
#include "constants.h"
#include "options.h"
#include "location.h"

extern State state;
extern unsigned short timer;
extern OptionsData optData;

void input_init(void) { 
    eos_start_read_keyboard(); 
}

void input_weather(void)
{
    switch (eos_end_read_keyboard())
    {
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
}

void input_forecast(void)
{
    switch (eos_end_read_keyboard())
    {
    case 0x83: // F3 LOCATION
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
}
