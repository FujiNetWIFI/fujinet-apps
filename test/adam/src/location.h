/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef LOCATION_H
#define LOCATION_H

#define MAX_CITY_SIZE (41)
#define MAX_IP (43)

#define LOCATION_VERSION "1.0"

typedef struct _location
{
    char version[4];
    char forceReadLocationByIP;
    char city[MAX_CITY_SIZE];
    char region_code[3];
    char country_code[3];
    char latitude[8];
    char longitude[8];
    char ip[43];
} Location;

extern static Location locData;

void location_defaults(void);
void location_print(void) ;
bool location_load(Location *o);
bool location_save(Location *o);
bool location_get_from_ip(char *c);
bool location_get_from_city(char *city, char *region, char *country);

void location(void);

#endif /* LOCATION_H */
