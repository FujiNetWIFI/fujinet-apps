/**
 * timestamp function
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef FTIME_H
#define FTIME_H

typedef struct _ftime
{
  unsigned short year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned char dow;
} Timestamp;

void timestamp(unsigned long t, Timestamp *ts);
const char *time_month(unsigned char month);
const char *time_dow(unsigned char dow);

#endif /* FTIME_H */
