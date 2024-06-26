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

#define LOCATION_VERSION "1.0"

typedef struct 
{
    char version[4];
    char city[MAX_CITY_SIZE];
    char region_code[3];
    char country_code[3];
    char latitude[13];
    char longitude[13];
    bool forceIPLocation;
    bool forceManualLocation;
    bool locationFound;
} Location;

bool location_request(void);

bool location_load(Location *o);
bool location_save(Location *o);
bool location_get_from_ip(void);
bool location_get_from_city(char *city, char *region, char *country);
bool location_get_from_position(char *latitude, char *longitude);
void location_print(void);
bool location_parse_from_ip(void);
void location_defaults(void);

void location(void);

extern Location locData;

#endif /* LOCATION_H */
