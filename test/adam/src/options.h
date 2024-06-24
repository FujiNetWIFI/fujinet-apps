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

#define OPTIONS_VERSION "1.1"

typedef enum _units
{
    METRIC,
    IMPERIAL,
    UNKNOWN
} Units;

typedef struct _optionsdata
{
    char version[4];
    unsigned char refreshIntervalMinutes;
    Units units;
    bool showRegion;
    char theme[12];
    unsigned char decimalPlaces;
} OptionsData;

extern static OptionsData optData;

bool options_load(OptionsData *o);
bool options_save(OptionsData *o);
void options_print();
void options_defaults(void);
void options(void);

#endif /* OPTIONS_H */
