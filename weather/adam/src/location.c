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
#include "screen.h"
#include "io.h"
#include "constants.h"

extern OptionsData optData;
Location locData;

static char tmp[256];

static const char *imperialCCodes[] = {"US", "GB", "IN", "IE", "CA", "AU", "HK", "NZ"};

extern State state;




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

    ak.cmd = 0xDE;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_LOCATION_KEY;
    memcpy(ak.data, o, sizeof(Location));

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK;
}

void location_parse_from_ip(void)
{
#ifdef DISPLAY_DEBUG
    int i;
#endif

    io_json_query("latitude", locData.latitude, sizeof(locData.latitude));

    io_json_query("longitude", locData.longitude, sizeof(locData.longitude));

    io_json_query("city", locData.city, sizeof(locData.city));

    io_json_query("country_code", locData.country_code, sizeof(locData.country_code));

    io_json_query("region_code", locData.region_code, sizeof(locData.region_code));

    //io_json_query("ip", locData.ip, sizeof(locData.ip));

#ifdef DISPLAY_DEBUG

    for (i=0; i<24; i++)
        cprintf("\n");

    cprintf(" latitude: %7s\n", locData.latitude);
    cprintf("longitude: %7s\n", locData.longitude);
    cprintf("     city: %s\n", locData.city);
    cprintf("   region: %2s\n", locData.region_code);
    cprintf("  country: %2s\n\n", locData.country_code);
    //cprintf("       ip: %s", locData.ip);
#endif    

}

void location_parse_from_city(char * region)
{
#ifdef DISPLAY_DEBUG
    int i;
#endif

    io_json_query("lat", locData.latitude,  sizeof(locData.latitude));
    io_json_query("lon", locData.longitude, sizeof(locData.longitude));
    io_json_query("name", locData.city, sizeof(locData.city));
    strcpy(locData.region_code, region);
    io_json_query("country", locData.country_code, sizeof(locData.country_code));

#ifdef DISPLAY_DEBUG

    for (i=0; i<24; i++)
        cprintf("\n");

    cprintf(" latitude: %7s\n", locData.latitude);
    cprintf("longitude: %7s\n", locData.longitude);
    cprintf("     city: %s\n", locData.city);
    cprintf("   region: %2s\n", locData.region_code);
    cprintf("  country: %2s\n\n", locData.country_code);
#endif    

}

// http://api.ipstack.com/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude
bool location_get_from_ip(char *c)
{
    char url[256];

    snprintf(url, sizeof(url),  "N:N:HTTP://%s/check?access_key=%s&fields=ip,region_code,country_code,city,latitude,longitude", 
                                IP_API,
                                IP_KEY);
    io_json_open(url);
    location_parse_from_ip();
    io_json_close();
}

//https://api.openweathermap.org/geo/1.0/direct?q=Borden-Carleton,PE,CA&apikey=2e8616654c548c26bc1c86b1615ef7f1
bool location_get_from_city(char *city, char *region, char *country)
{
    char url[256];

    snprintf(url, sizeof(url),  "N:HTTP://%s/geo/1.0/direct?q=%s,%s,%s&appkey=%s", 
                                OW_API, city, region, country,
                                OW_KEY);
    io_json_open(url);
    // [{"name":"Borden-Carleton","lat":46.2533283,"lon":-63.6931751,"country":"CA","state":"Prince Edward Island"}]

    location_parse_from_city(region);
    io_json_close();
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
    screen_location_detect();
    location_get_from_ip(tmp);
    

    if (optData.units == UNKNOWN)
    {
        if (locationIsImperial())
            optData.units = IMPERIAL;
        else
            optData.units = METRIC;
    }

    if (!location_save(&locData))
        screen_options_could_not_save();
    options_save(optData);
}

void location(void)
{
    if (!location_load(&locData))
    {
        if (optData.detectLocation == true)
            location_detect();
    }

    if (locData.latitude[0] != 0x00)
        state = WEATHER;
}
