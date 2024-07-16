/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * @brief Satellite position fetch functions
 */

#ifndef FETCH_H
#define FETCH_H

/**
 * @brief fetch satellite telemetry, and return as ints and strings
 * @param lon pointer to integer longitude
 * @param lat pointer to integer latitude
 * @param lon_s pointer to longitude string (for showing floating point val)
 * @param lat_s pointer to latitude string (for showing floating point val)
 */
void fetch(int *lon, int *lat, char *lon_s, char *lat_s);

#endif /* FETCH_H */
