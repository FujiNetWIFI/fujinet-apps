/**
 * Weather 
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef FORECAST_H
#define FORECAST_H

#define MAX_FORECAST (25)

typedef struct
{
  char date[8];
  unsigned char icon;
  char dow[4];
  char hi[8];
  char lo[8];
  char pressure[10];
  char wind[27];
  char rain[12];
  char snow[12];
  char pop[8];
  char desc[MAX_FORECAST];
} ForecastData;

void forecast(void);
void forecast_parse(unsigned char i, ForecastData *f);
void screen_forecast_init();

extern unsigned char forecast_offset;

#endif /* FORECAST_H */
