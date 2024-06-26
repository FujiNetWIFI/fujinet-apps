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

#define OPTIONS_VERSION "1.0"

#define CHECK_TIME_FREQUENCY (4000)

typedef struct _optionsdata
{
    char version[4];
    long refreshIntervalMinutes;
    Units units;
    bool showRegion;
    char theme[12];
    int maxPrecision;
} OptionsData;

bool options_load(OptionsData *o);
bool options_save(OptionsData *o);
void options_defaults(void);

void options(void);

extern OptionsData optData;

#endif /* OPTIONS_H */
