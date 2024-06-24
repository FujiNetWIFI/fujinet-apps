/**
 * Weather / main.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "init.h"
#include "io.h"
#include "welcome.h"
#include "options.h"
#include "location.h"
#include "weather.h"
#include "constants.h"
#include "forecast.h"
#include "screen.h"
#include "state.h"


void main(void)
{
    int i;

    while (1)
    {
        switch (state)
        {
        case INIT:
            init();
            state = WELCOME;
            break;
        case WELCOME:
            welcome();
            state = TIME;
            break;
        case TIME:
            //screen_current_time();
            state = OPTIONS;
            break;
        case OPTIONS:
            options();
            state = LOCATION;
            break;
        case LOCATION:
            location(); // will handle state change 
            break;
        case WEATHER:
            weather(); // will handle state change
            break;
        case FORECAST:
            forecast(); // will handle state change
            break;
        case DONE:
            break;
        }
    }
}
