/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief Satellite position fetch functions
 */

#include <c64.h>
#include <cbm.h>
#include <string.h>
#include <stdlib.h>

#define LFN 2     // The logical file number to use for I/O
#define DEV 12    // The network device #
#define SAN 2     // The secondary address (SA) to use on DEV.

const char url[]="HTTP://api.open-notify.org/iss-now.json";

const char cmd_parse[]="parse,2";
const char lon_query[] = "q,2,/iss_position/longitude";
const char lat_query[] = "q,2,/iss_position/latitude";
const char ts_query[] = "q,2,/timestamp";

/**
 * @brief fetch satellite telemetry, and return as ints and strings
 * @param lon pointer to integer longitude
 * @param lat pointer to integer latitude
 * @param lon_s pointer to longitude string (for showing floating point val)
 * @param lat_s pointer to latitude string (for showing floating point val)
 * @param ts pointer to timestamp
 */
void fetch(int *lon, int *lat, char *lon_s, char *lat_s, unsigned long *ts)
{
  char tmp[80];
  
  memset(lon_s,0,16);
  memset(lat_s,0,16);

  cbm_open(LFN,DEV,SAN,url);

  cbm_write(LFN,cmd_parse,sizeof(cmd_parse));

  cbm_write(LFN,ts_query,sizeof(ts_query));
  cbm_read(LFN,tmp,sizeof(tmp));
  *ts = atol(tmp);

  cbm_write(LFN,lon_query,sizeof(lon_query));
  cbm_read(LFN,tmp,sizeof(tmp));
  strcpy(lon_s,tmp);
  *lon = atoi(tmp);

  cbm_write(LFN,lat_query,sizeof(lat_query));
  cbm_read(LFN,tmp,sizeof(tmp));
  strcpy(lat_s,tmp);
  *lat = atoi(tmp);

  cbm_close(LFN);
}
