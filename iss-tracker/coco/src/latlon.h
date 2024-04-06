/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Latitude/Longitude transform functions
 */

#ifndef LATLON_H
#define LATLON_H

/**
 * @brief return X position for given longitude 
 * @param x the longitude in degrees (-180 to +180)
 * @return The X position
 */
unsigned char lon_to_x(int x);

/**
 * @brief return Y position for given latitude 
 * @param x the longitude in degrees (-90 to +90)
 * @return The X position
 */
unsigned char lat_to_y(int y);

#endif /* LATLON_H */
