/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Fetch current position from web
 */

#include <cmoc.h>
#include <coco.h>
#include "net.h"
#include "pmode3.h"
#include "globals.h"

#define HTTP_GET 12
#define NO_TRANSLATION 0

#define JSON 1

#define SUCCESS 1

#define API_ENDPOINT "N:HTTP://api.open-notify.org/iss-now.json"

#define QUERY_LONGITUDE "/iss_position/longitude"
#define QUERY_LATITUDE "/iss_position/latitude"
#define QUERY_TIMESTAMP "/timestamp"

/**
 * @brief fetch individual json element and populate vars.
 */
void fetch_json(const char *qs, char *s, int *i)
{
    NetworkStatus ns;

    // Set query
    net_set_json_query(0,qs);

    // Get # of bytes waiting
    net_status(0,&ns);

    // read into buffer
    net_read(0,(unsigned char *)s,ns.bytesWaiting);

    // Convert string to integer
    *i=atoi(s);
}

/**
 * @brief fetch individual json element and populate vars.
 */
void fetch_json_timestamp(const char *qs, unsigned long *l)
{
    NetworkStatus ns;
    char tsi[16];
    
    // Set query
    net_set_json_query(0,qs);

    // Get # of bytes waiting
    net_status(0,&ns);

    // read into buffer
    net_read(0,(unsigned char *)tsi,ns.bytesWaiting);

    // Convert string to integer
    *l=atol(tsi);
}

/**
 * @brief Fetch JSON data from web endpoint, and populate vars
 */
void fetch(void)
{
    byte err;

    err = net_open(0,HTTP_GET,NO_TRANSLATION,API_ENDPOINT);

    if (err != SUCCESS)
    {
        puts(0,160,1,"OPEN ERROR.");
        return;
    }

    // Set channel mode to JSON
    net_set_channel_mode(0,JSON);

    err = net_parse_json(0);

    if (err != SUCCESS)
    {
        puts(0,160,1,"JSON PARSE ERROR.");
        return;
    }

    fetch_json(QUERY_LONGITUDE,&lon_s,&lon_i);
    fetch_json(QUERY_LATITUDE,&lat_s,&lat_i);
    fetch_json_timestamp(QUERY_TIMESTAMP,&ts);
    
    net_close(0);
    
}
