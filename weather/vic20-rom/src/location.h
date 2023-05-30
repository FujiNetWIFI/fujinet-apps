/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose ip geolocation
 */

#ifndef LOCATION_H
#define LOCATION_H

typedef struct _location
{
  char city[41];
  char region_code[3];
  char country_code[3];
  char latitude[18];
  char longitude[18];
} Location;

void location(void);

#endif /* LOCATION_H */
