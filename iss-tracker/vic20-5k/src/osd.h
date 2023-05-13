/**
 * @brief   ISS Tracker for 5 kilobyte VIC-20
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see COPYING for details
 */

#ifndef OSD_H
#define OSD_H

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 */
void osd(char *lon_s, char *lat_s);

#endif /* OSD_H */
