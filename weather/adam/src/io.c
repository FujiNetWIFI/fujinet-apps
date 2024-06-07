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
#include "constants.h"
#include "io.h"
#include "options.h"
#include "ftime.h"
#include "utils.h"

#define NET_DEV  0x09
#define FUJI_DEV 0x0F

#define READ_WRITE 12

extern OptionsData optData;
extern Location locData;

static unsigned char response[1024];
static char line[256];

unsigned long dt, sunrise, sunset;
unsigned short timer = 65535;

extern char date_txt[32];
extern char sunrise_txt[16];
extern char sunset_txt[16];
extern char time_txt[16];
extern char temp[24];
extern char timezone[24];
extern char feels_like[16];
extern unsigned short timezone_offset;
extern char pressure[14];
extern char humidity[16];
extern char dew_point[16];
extern char clouds[16];
extern char visibility[16];
extern char wind_speed[16];
extern char wind_dir[3];
extern char wind_txt[16];
extern char description[24];
extern char loc[48];
extern unsigned char icon;

bool io_options_load(OptionsData *o)
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
    if (eos_read_character_device(FUJI_DEV, response, 1024) == 0x80)
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

bool io_options_save(OptionsData *o)
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

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == 0x80;
}

bool io_location_load(Location *o)
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
    if (eos_read_character_device(FUJI_DEV, response, 1024) == 0x80)
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

bool io_location_save(Location *o)
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

    return eos_write_character_device(FUJI_DEV, ak, sizeof(ak)) == 0x80;
}

bool io_location_get_from_ip(char *c)
{
    unsigned char res;
    char cc = 'C';

    struct
    {
        unsigned char cmd;
        unsigned char aux1;
        unsigned char aux2;
        unsigned char url[160];
    } co;

    // Set up open
    memset(co, 0, sizeof(co));
    co.cmd = 'O';
    co.aux1 = READ_WRITE;
    co.aux2 = 0;
    strcpy(co.url, "N:HTTP://");
    strcat(co.url, IP_API);
    strcat(co.url, "/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude");

    // Do open
    res = eos_write_character_device(NET_DEV, co, sizeof(co));

    if (res != 0x80)
        return false; // error

    while ((res = eos_read_character_device(NET_DEV, response, 1024)) == 0x80)
    {
        strcat(line, response);
        memset(response, 0, sizeof(response));
    }

    strcpy(c, line);

    // Close connection
    eos_write_character_device(NET_DEV, &cc, 1);
}

bool io_json_query(char *q, char *d)
{
    struct _qcm
    {
        char cmd;
        char query[128];
    } qcm;

    unsigned char r = 0;

    qcm.cmd = 'Q';
    strcpy(qcm.query, q);
    r = eos_write_character_device(NET_DEV, (unsigned char *)qcm, sizeof(qcm));

    if (r != 0x80)
        return false;

    r = eos_read_character_device(NET_DEV, (unsigned char *)d, 1024);

    if (r != 0x80)
        return false;

    return true;
}

bool io_weather(char *j)
{
    char units[14];
    char cc = 'C';
    unsigned char res;

    struct
    {
        char cmd;
        char aux1;
        char aux2;
        char url[255];
    } co;

    struct
    {
        char cmd;
        char mode;
    } cscm;

    unsigned char tmp[128];

    if (optData.units == METRIC)
        strcpy(units, "metric");
    else if (optData.units == IMPERIAL)
        strcpy(units, "imperial");

    // Set up open
    co.cmd = 'O';
    co.aux1 = READ_WRITE;
    co.aux2 = 0;
    snprintf(co.url, sizeof(co.url), "N:HTTP://%s//data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts,daily&units=%s&appid=%s", OW_API, locData.latitude, locData.longitude, units,
             "2e8616654c548c26bc1c86b1615ef7f1");

    // Do open
    res = eos_write_character_device(NET_DEV, co, sizeof(co));

    if (res != 0x80)
        return false;

    // Set channel mode to JSON
    cscm.cmd = 0xFC;
    cscm.mode = 1;
    res = eos_write_character_device(NET_DEV, cscm, sizeof(cscm));

    if (res != 0x80)
        return false;

    // Send to parser.
    res = eos_write_character_device(NET_DEV, "P", 1);
    if (res != 0x80)
        return false;

    // Grab the relevant bits
    if (!io_json_query("/current/dt", tmp))
        return false;
    dt = atol(tmp);

    if (!io_json_query("/timezone", tmp))
        return false;

    ellipsizeString(&tmp[0], &timezone[0], sizeof(timezone));

    if (!io_json_query("/current/sunrise", tmp))
        return false;
    sunrise = atol(tmp);

    if (!io_json_query("/current/sunset", tmp))
        return false;
    sunset = atol(tmp);

    if (!io_json_query("/current/temp", tmp))
        return false;
    sprintf(temp, "%s*%c", tmp, optData.units == IMPERIAL ? 'F' : 'C');

    if (!io_json_query("/current/feels_like", tmp))
        return false;
    sprintf(feels_like, "%s*%c", tmp, optData.units == IMPERIAL ? 'F' : 'C');

    if (!io_json_query("/timezone_offset", tmp))
        return false;
    timezone_offset = atoi(tmp);

    if (!io_json_query("/current/pressure", tmp))
        return false;

    if (optData.units == IMPERIAL)
        weather_hpa_to_inhg(tmp);

    sprintf(pressure, "%s %s", tmp, optData.units == IMPERIAL ? "\"Hg" : "mPa");

    if (!io_json_query("/current/humidity", tmp))
        return false;
    sprintf(humidity, "%s%%", tmp);

    if (!io_json_query("/current/dew_point", tmp))
        return false;
    sprintf(dew_point, "%s deg %c", tmp, optData.units == IMPERIAL ? 'F' : 'C');

    if (!io_json_query("/current/clouds", tmp))
        return false;
    sprintf(clouds, "%s%%", tmp);

    if (!io_json_query("/current/visibility", tmp))
        return false;
    sprintf(visibility, "%d %s", atoi(tmp) / 1000, optData.units == IMPERIAL ? "mi" : "km");

    if (!io_json_query("/current/wind_speed", tmp))
        return false;
    sprintf(wind_speed, "%s %s", json_part, optData.units == IMPERIAL ? "mph" : "kph");

    if (!io_json_query("/current/wind_deg", tmp))
        return false;
    sprintf(wind_dir, "%s", degToDirection(atoi(json_part)));

    if (!io_json_query("/current/description", tmp))
        return false;
    sprintf(description, "%s", json_part);
    strcpy(description, strupr(description));

    if (!io_json_query("/current/icon", tmp))
        return false;
    icon = icon_get(tmp);

    // Close connection
    eos_write_character_device(NET_DEV, &cc, 1);

    return true;
}

bool io_forecast(char *j)
{
    Timestamp ts;
    char units[14];
    char cc = 'C';
    unsigned char res;
    struct
    {
        char cmd;
        char aux1;
        char aux2;
        char url[255];
    } co;

    struct
    {
        char cmd;
        char mode;
    } cscm;

    if (optData.units == METRIC)
        strcpy(units, "metric");
    else if (optData.units == IMPERIAL)
        strcpy(units, "imperial");

    // Set up open
    co.cmd = 'O';
    co.aux1 = READ_WRITE;
    co.aux2 = 0;
    snprintf(co.url, sizeof(co.url), "N:HTTP://%s//data/2.5/onecall?lat=%s&lon=%s&exclude=minutely,hourly,alerts,current&units=%s&appid=%s", OW_API, locData.latitude, locData.longitude, units,
             "2e8616654c548c26bc1c86b1615ef7f1");

    // Do open
    res = eos_write_character_device(NET_DEV, co, sizeof(co));

    if (res != 0x80)
        return false;

    // Set channel mode to JSON
    cscm.cmd = 0xFC;
    cscm.mode = 1;
    res = eos_write_character_device(NET_DEV, cscm, sizeof(cscm));

    if (res != 0x80)
        return false;

    // Send to parser.
    res = eos_write_character_device(NET_DEV, "P", 1);
    if (res != 0x80)
        return false;

    // Grab the relevant bits
    if (!io_json_query("/current/dt", tmp))
        return false;
    dt = atol(tmp);

    // Close connection
    eos_write_character_device(NET_DEV, &cc, 1);

    return true;
}
