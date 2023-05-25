/**
 * #FujiNet ISS tracker for C64
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 *
 * Satellite display
 */

#ifndef SATELLITE_H
#define SATELLITE_H

/**
 * @brief Display satellite at lon, lat
 * @param lon Integer longitude (-180, 180 deg)
 * @param lat Integer latitude  (-180, 180 deg)
 */
void satellite(int lon, int lat);

#endif /* SATELLITE_H */
