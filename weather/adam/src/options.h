/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef enum _units
  {
   METRIC,
IMPERIAL,
   UNKNOWN
  } Units;

typedef struct _optionsdata
{
  char apiKeyOW[32];
  unsigned char refreshInterval;
  Units units;
  bool showRegion;
  bool detectLocation;
  char theme[12];
  unsigned char maxPrecision;
  char unused[10];
} OptionsData;

void options_save(OptionsData *o);
void options(void);

#endif /* OPTIONS_H */
