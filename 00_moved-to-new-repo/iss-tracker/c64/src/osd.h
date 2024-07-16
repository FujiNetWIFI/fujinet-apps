
#ifndef OSD_H
#define OSD_H

/**
 * @brief Update lower 4 lines of display
 * @param lon_s string of longitude
 * @param lat_s string of latitude
 * @param ts 32-bit timestamp
 */
void osd(char *lon_s, char *lat_s, unsigned long ts);

#endif /* OSD_H */
