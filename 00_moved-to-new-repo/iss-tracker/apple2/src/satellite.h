/**
 * ISS Tracker
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#ifndef SATELLITE_H
#define SATELLITE_H

bool satellite_fetch(int *lon, int *lat, char *lon_s, char *lat_s, unsigned long *ts);
void satellite_draw(int lon, int lat);
void satellite_erase(void);

#endif /* SATELLITE_H */
