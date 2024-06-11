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

OptionsData optData;

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
    memcpy(ak.data, o, sizeof(OptionsData));

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK;
}



void options_defaults(void)
{
    screen_options_init_not_found();

    memset(optData.apiKeyOW, 0, sizeof(optData.apiKeyOW));
    strcpy(optData.apiKeyOW, "2e8616654c548c26bc1c86b1615ef7f1");
    optData.refreshInterval = DEFAULT_REFRESH;
    optData.units = UNKNOWN;
    optData.showRegion = false;
    optData.detectLocation = true;
    optData.maxPrecision = 1;
    strcpy(optData.theme, "DEFAULT.THM");
    if (! options_save(optData))
        screen_options_could_not_save();
}

void options_init(void)
{
    screen_options_init();

    if (!options_load(optData))
        options_defaults();
}

void options(void) { options_init(); }
