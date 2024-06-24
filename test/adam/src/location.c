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
#include <string.h>
#include <conio.h>
#include "location.h"
#include "options.h"
#include "io.h"
#include "constants.h"


static char tmp[256];

static const char *imperialCCodes[] = {"US", "GB", "IN", "IE", "CA", "AU", "HK", "NZ"};

extern State state;

void location_print(void)
{
    cprintf("location print:\nVersion:%s\nCity:%s\nLat:%s\nLong:%s\nIP:%s\n", locData.version, locData.city, locData.latitude, locData.longitude,locData.ip);
}

/** LOCATION  */

bool location_load(Location *o)
{
    bool ret;
    struct
    {
        unsigned char cmd;
        unsigned short creator;
        unsigned char app;
        unsigned char key;
    } ak;

    cprintf("Size of ak %d\n", sizeof(ak));
    cprintf("Size of Location %d\n", sizeof(Location));

    ak.cmd = 0xDD;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_LOCATION_KEY;

    eos_write_character_device(FUJI_DEV, ak, sizeof(ak));
    if (eos_read_character_device(FUJI_DEV, response, sizeof(response)) == ACK)
    {
        DCB *dcb = eos_find_dcb(FUJI_DEV);

        if (dcb->len == 1)
            return false;

        memcpy(o, response, sizeof(Location));
        ret = true;
    }
    else
        ret = false;

    return ret;
}

bool location_save(Location *o)
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

    cprintf("Size of new ak %d\n", sizeof(ak));

    ak.cmd = 0xDE;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_LOCATION_KEY;

    strcpy(o->version, LOCATION_VERSION);
    memcpy(ak.data, o, sizeof(Location));

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK;
}

void location_parse_from_ip(void)
{
#ifdef DISPLAY_DEBUG
    int i;
#endif
    char temp[1024];

    strcpy(temp, "1234567890");

    cprintf("sizeof: %d\n", sizeof(locData.latitude));
    cprintf("temp before call: '%s'\n", temp);

    io_json_query("latitude", temp, sizeof(locData.latitude));
    cprintf("temp after call: '%s'\n", temp);

}



void location_defaults(void)
{
    memset(&locData, 0, sizeof(locData));
}

// http://api.ipstack.com/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude
bool location_get_from_ip(char *c)
{
    char url[256];

    snprintf(url, sizeof(url),  "N:HTTP://%s/check?access_key=%s&fields=ip,region_code,country_code,city,latitude,longitude", 
                                IP_API,
                                IP_KEY);

    if (io_json_open(url) == 0)
    {
        location_parse_from_ip();
        io_json_close();
    } else
    {
        cprintf("failed to open\n");
    }
}



bool locationIsImperial(void)
{
    unsigned char i = 0;

    for (i = 0; i < sizeof(imperialCCodes); i++)
        if (strncmp(locData.country_code, imperialCCodes[i], 2) == 0)
            return true;

    return false;
}


void location_detect(void)
{
    location_get_from_ip(tmp);
    
    if (optData.units == UNKNOWN)
    {
        if (locationIsImperial())
            optData.units = IMPERIAL;
        else
            optData.units = METRIC;
    }

    location_save(&locData);

    options_save(optData);
}

void location(void)
{
    if (!location_load(&locData))
    {
        if (locData.forceReadLocationByIP)
            location_detect();
    }

    if (locData.latitude[0] != 0x00)
        state = WEATHER;
}
