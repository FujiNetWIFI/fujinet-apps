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
#include "io.h"

static OptionsData optData;

void options_print()
{

    cprintf("Options print\n");
    cprintf("Version:%s\n", optData.version);
    cprintf("refresh: %d\n", (int)optData.refreshIntervalMinutes);
    cprintf("units:%d\n", (int)optData.units);
    cprintf("theme:%s\n", optData.theme);
    cprintf("maxPrecision:%d\n", (int)optData.decimalPlaces);

}

bool options_load(OptionsData *o)
{
    bool ret;
    struct
    {
        unsigned char cmd;
        unsigned short creator;
        unsigned char app;
        unsigned char key;
    } ak;

    ak.cmd = 0xDD;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_CONFIG_KEY;

    eos_write_character_device(FUJI_DEV, ak, sizeof(ak));
    if (eos_read_character_device(FUJI_DEV, response, sizeof(response)) == ACK)
    {
        DCB *dcb = eos_find_dcb(FUJI_DEV);

        if (dcb->len == 1)
            return false;

        memcpy(o, response, sizeof(OptionsData));
        o->version[sizeof(o->version) - 1] = '\0';

        if (strncmp(o->version, OPTIONS_VERSION, sizeof(o->version)) != 0)
            ret = false;
        else
            ret = true;

    }
    else
        ret = false;

    return ret;
}

bool options_save(OptionsData *o)
{
    bool ret;
    struct
    {
        unsigned char cmd;
        unsigned short creator;
        unsigned char app;
        unsigned char key;
        char data[64];
    } ak;

    ak.cmd = 0xDE;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_CONFIG_KEY;

    strcpy(optData.version, OPTIONS_VERSION);
    memcpy(ak.data, o, sizeof(OptionsData));

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK;
}



void options_defaults(void)
{
    optData.refreshIntervalMinutes = DEFAULT_REFRESH;
    optData.units = UNKNOWN;
    optData.showRegion = false;
    optData.refreshIntervalMinutes = DEFAULT_REFRESH;
    optData.decimalPlaces = 2;
    strcpy(optData.theme, "DEFAULT.THM");
    options_save(optData);

}

void options_init(void)
{

    if (!options_load(optData))
        options_defaults();
}

void options(void) { options_init(); }
