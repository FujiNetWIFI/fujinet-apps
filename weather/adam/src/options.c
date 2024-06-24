/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */
#include <stdbool.h>
#include <eos.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "options.h"
#include "screen.h"
#include "io.h"
#include "screen.h"

OptionsData optData;

void options_print()
{
    cprintf("Options print\n");
    cprintf("refresh: %d\n", (int) optData.refreshIntervalMinutes);
    cprintf("units:%d\n", (int) optData.units);
    cprintf("theme:%s\n", optData.theme);
    cprintf("maxPrecision:%d\n", (int) optData.maxPrecision);
    csleep(DEBUG_DELAY);
}
bool options_load(OptionsData *o)
{
    int num_found = 0;
    bool ret = false;
    FUJI_APP ak;
    char temp[MAX_FORECAST];
    char *p;
    int units;

    ak.cmd = 0xDD;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_CONFIG_KEY;

    eos_write_character_device(FUJI_DEV, ak, sizeof(ak));
    if (eos_read_character_device(FUJI_DEV, response, sizeof(response)) == ACK)
    {
        DCB *dcb = eos_find_dcb(FUJI_DEV);

        if (dcb->len != 1)
        {
            p = io_get_next_string(response, o->version, sizeof(o->version));
            if (strncmp(o->version, OPTIONS_VERSION, sizeof(o->version)) == 0)
            {
                p = io_get_next_long(p, &o->refreshIntervalMinutes);
                p = io_get_next_int(p, &units);
                o->units = units;
                p = io_get_next_bool(p, &o->showRegion);
                p = io_get_next_string(p, o->theme, sizeof(o->theme));
                p = io_get_next_int(p, &o->maxPrecision);
                ret = (p != NULL);
            }
        }
    }

    return ret;
}

bool options_save(OptionsData *o)
{
    bool ret;
    FUJI_APP_DATA ak;

    screen_options_saving();

    ak.cmd = 0xDE;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_CONFIG_KEY;

    strncpy2(o->version, OPTIONS_VERSION, sizeof(o->version));

    snprintf(response,  sizeof(response), "%s|%ld|%d|%d|%s|%d\n",
        o->version,
        o->refreshIntervalMinutes,
        o->units,
        o->showRegion,
        o->theme,
        o->maxPrecision);

    strncpy2(ak.data, response, sizeof(ak.data));
    ret =  (eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK);

    return ret;
}



void options_defaults(void)
{
    strncpy2(optData.version, OPTIONS_VERSION, sizeof(optData.version));
    optData.refreshIntervalMinutes = DEFAULT_REFRESH;
    optData.units = UNKNOWN;
    optData.showRegion = false;
    optData.maxPrecision = 1;
    strncpy2(optData.theme, "DEFAULT.THM", sizeof(optData.theme));

    if (! options_save(optData))
        screen_options_could_not_save();
}

void options_init(void)
{
    screen_options_init();

    if (!options_load(optData))
    {
        screen_options_load_failed();
        options_defaults();
    } 
        
}

void options(void) { 
    options_init(); 
}
