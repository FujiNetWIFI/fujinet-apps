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

typedef struct _location
{
  char city[41];
  char region_code[3];
  char country_code[3];
  char latitude[8];
  char longitude[8];
} Location;

void location(void);

#endif /* LOCATION_H */
