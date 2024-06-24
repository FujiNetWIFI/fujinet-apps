/**
 * Weather / location.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */
#include <stdbool.h>
#include <string.h>
#include <eos.h>
#include <string.h>
#include <conio.h>
#include "location.h"
#include "options.h"
#include "screen.h"
#include "io.h"
#include "constants.h"
#include "state.h"

Location locData;

static const char *imperialCCodes[] = {"US", "GB", "IN", "IE", "CA", "AU", "HK", "NZ"};

extern bool forceRefresh;

/** LOCATION  */

bool location_request(void)
{
    if (screen_location(&locData, &locData.forceIPLocation, &locData.forceManualLocation))
    {
        locData.locationFound = ! locData.forceIPLocation;
        if (!location_save(&locData))
            screen_location_could_not_save();
        else
            screen_location_saved();
    }
}


void location_defaults(void)
{
    screen_location_load_defaults();

    strncpy2(locData.version,      LOCATION_VERSION, sizeof(locData.version));

    strncpy2(locData.city,         "Borden-Carleton", sizeof(locData.city));

    strncpy2(locData.region_code,  "PE", sizeof(locData.region_code));

    strncpy2(locData.country_code, "CA", sizeof(locData.country_code));

    strncpy2(locData.latitude,    "46.2533283", sizeof(locData.latitude));

    strncpy2(locData.longitude,   "-63.6931751", sizeof(locData.longitude));

    locData.forceIPLocation = true;
    locData.forceManualLocation = false;
    locData.locationFound = false;
    
    if (! location_save(&locData))
        screen_location_could_not_save();
    else
        screen_location_success();
}

void location_print(void)
{
    cprintf("location print:\nVersion:%s\nCity:%s\nLat:%s\nLong:%s\n",
        locData.version,
        locData.city,
        locData.latitude,
        locData.longitude);

    cprintf("forceIP: %d\nlocationFound:%d\n", locData.forceIPLocation, locData.locationFound);
    csleep(DEBUG_DELAY);
}

bool location_load(Location *l)
{
    int num_found = 0;
    bool ret = false;
    FUJI_APP ak;
    char *p;
    char temp[MAX_CITY_SIZE];

    ak.cmd = 0xDD;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_LOCATION_KEY;

    eos_write_character_device(FUJI_DEV, ak, sizeof(ak));
    if (eos_read_character_device(FUJI_DEV, response, sizeof(response)) == ACK)
    {
        DCB *dcb = eos_find_dcb(FUJI_DEV);

        if (dcb->len != 1)
        {
            p = io_get_next_string(response, l->version, sizeof(l->version));
            if (strncmp(l->version, LOCATION_VERSION, sizeof(l->version)) == 0)
            {
                p = io_get_next_string(p, l->city, sizeof(l->city));
                p = io_get_next_string(p, l->region_code, sizeof(l->region_code));
                p = io_get_next_string(p, l->country_code, sizeof(l->country_code));
                p = io_get_next_string(p, l->latitude, sizeof(l->latitude));
                p = io_get_next_string(p, l->longitude, sizeof(l->longitude));
                p = io_get_next_bool(p, &l->forceIPLocation);
                p = io_get_next_bool(p, &l->forceManualLocation);
                p = io_get_next_bool(p, &l->locationFound);
                ret = (p != NULL);
            }
        }
    }

    return ret;
}

bool location_save(Location *l)
{
    bool ret;
    FUJI_APP_DATA ak;

    screen_location_saving();

    ak.cmd = 0xDE;
    ak.creator = APPKEY_CREATOR_ID;
    ak.app = APPKEY_APP_ID;
    ak.key = APPKEY_LOCATION_KEY;

    strncpy2(l->version, LOCATION_VERSION, sizeof(l->version));
    // use | as the data delimitor

    snprintf(response, sizeof(response),"%s|%s|%s|%s|%s|%s|%d|%s|%d\n",
        l->version, 
        l->city, 
        l->region_code, 
        l->country_code,
        l->latitude, 
        l->longitude, 
        (int) l->forceIPLocation,
        (int) l->forceManualLocation, 
        (int) l->locationFound);

    strncpy2(ak.data, response, sizeof(ak.data));
    ret =  (eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == ACK);

    return ret;
}



/******************************************************************** */

bool location_parse_from_position(void)
{

    if (io_json_query("lat", locData.latitude,  sizeof(locData.latitude)))
    {
        return false;
    }
    debug_print(locData.latitude);
    if (io_json_query("lon", locData.longitude, sizeof(locData.longitude)))
    {
        return false;
    }
    debug_print(locData.longitude);

    if (io_json_query("display_name", locData.city, sizeof(locData.city)))
    {
        return false;
    }
    debug_print(locData.longitude);
    strcpy(locData.region_code, "");
    
    if (io_json_query("address/country_code", locData.country_code, sizeof(locData.country_code)))
    {
        return false;
    }
   
    return true;
    
}

//https://locationiq.com/v1/reverse?key=pk.e5e4ed6155f82872e7303a83744048a1&lat=46.2533283&lon=-63.6931751&format=json&zoom=10&
bool location_get_from_position(char *latitude, char *longitude)
{
    char url[256];
    bool ret = false;

    screen_location_position_detect();

    snprintf(url, sizeof(url),  "N:HTTPS://%s/v1/reverse?key=%s&lat=%s&lon=%s&format=json&zoom=10&", 
                                LIQ_API,
                                LIQ_KEY,
                                latitude,
                                longitude);
    if (io_json_open(url) == 0)
    {
        ret = location_parse_from_position();
        io_json_close();
    }

    return ret;    
}

/******************************************************************** */

bool location_parse_from_ip(void)
{
#ifdef DISPLAY_DEBUG
    int i;
#endif
    unsigned char ip[43];
    unsigned char success[10];
    bool detected = false;

    io_json_query("success", success, sizeof(success));
    if (stricmp(success, "true") == 0)
    {
        detected = true;

        io_json_query("latitude", locData.latitude, sizeof(locData.latitude));

        io_json_query("longitude", locData.longitude, sizeof(locData.longitude));

        io_json_query("city", locData.city, sizeof(locData.city));

        io_json_query("country_code", locData.country_code, sizeof(locData.country_code));

        io_json_query("region_code", locData.region_code, sizeof(locData.region_code));

        io_json_query("ip", ip, sizeof(ip));

    }

    return detected;

}

// http://api.ipstack.com/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude
bool location_get_from_ip(void)
{
    char url[256];
    bool detected = false;

    snprintf(url, sizeof(url),  "N:HTTP://%s/check?access_key=%s&fields=ip,region_code,country_code,city,latitude,longitude", 
                                IP_API,
                                IP_KEY);

    if (io_json_open(url) == 0)
    {
        detected = location_parse_from_ip();
        io_json_close();
    }

    return detected;
}

/******************************************************************** */


bool locationIsImperial(void)
{
    unsigned char i = 0;

    for (i = 0; i < sizeof(imperialCCodes); i++)
        if (strncmp(locData.country_code, imperialCCodes[i], 2) == 0)
            return true;

    return false;
}


bool location_ip_detect(void)
{
    bool detected = false;
    screen_location_ip_detect();

    detected = location_get_from_ip();
    
    if (optData.units == UNKNOWN)
    {
        if (locationIsImperial())
            optData.units = IMPERIAL;
        else
            optData.units = METRIC;
    }

    if (!location_save(&locData))
    {
        screen_location_could_not_save();
    }
    return detected;

}

void location(void)
{
static bool firstTime = true;

    if (firstTime)
    {
        if (!location_load(&locData))
        {
            screen_location_load_failed();
            location_defaults();

        } else
        {
            firstTime = false;
        }
    }

    if (locData.forceManualLocation == true)
    {
        locData.forceIPLocation = false;
        locData.forceManualLocation = false;
        location_request();
        forceRefresh = true;
    }

    if (locData.forceIPLocation == true)
    {
        if (! location_ip_detect() )
        {
            screen_location_could_not_ip_detect();
            locData.forceManualLocation = true;
        } else
        {
            location_save(&locData);
        }
        locData.forceIPLocation = false;
    }

    if (locData.locationFound)
    {
        state = WEATHER;
        
    } else
    {
        screen_location_could_not_detect();
        location_defaults();
    }
    //else
    //  call location again...
}
