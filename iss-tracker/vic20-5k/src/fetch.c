/**
 * #FujiNet ISS tracker for 5K VIC20
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief Satellite position fetch functions
 */

#include <vic20.h>
#include <cbm.h>
#include <string.h>
#include <stdlib.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 16    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.
#define CMD 15    // The secondary address for the Command channel

// Don't translate underscore to petscii glyph in string literals
#pragma charmap (0x5F, 0x5F);

const char url[]="http://api.open-notify.org/iss-now.json";

const char cmd_parse[]="jsonparse,2";
const char lon_query[] = "jq,2,/iss_position/longitude";
const char lat_query[] = "jq,2,/iss_position/latitude";
const char ts_query[] = "jq,2,/timestamp";

static char tmp[32];

/**
 * @brief fetch satellite telemetry, and return as ints and strings
 * @param lon pointer to integer longitude
 * @param lat pointer to integer latitude
 * @param lon_s pointer to longitude string (for showing floating point val)
 * @param lat_s pointer to latitude string (for showing floating point val)
 * @param ts pointer to timestamp
 */
void fetch(int *lon, int *lat, char *lon_s, char *lat_s)
{
  memset(tmp,0,sizeof(tmp));
  memset(lon_s,0,16);
  memset(lat_s,0,16);

  cbm_open(CMD,DEV,CMD,"");
  cbm_open(LFN,DEV,SAN,url);

  cbm_write(CMD,cmd_parse,sizeof(cmd_parse));

  cbm_write(CMD,lon_query,sizeof(lon_query));
  memset(tmp,0,sizeof(tmp));
  cbm_read(LFN,tmp,sizeof(tmp));
  strcpy(lon_s,tmp);
  *lon = atoi(tmp);

  cbm_write(CMD,lat_query,sizeof(lat_query));
  memset(tmp,0,sizeof(tmp));
  cbm_read(LFN,tmp,sizeof(tmp));
  strcpy(lat_s,tmp);
  *lat = atoi(tmp);

  cbm_close(LFN);
  cbm_close(CMD);
}
