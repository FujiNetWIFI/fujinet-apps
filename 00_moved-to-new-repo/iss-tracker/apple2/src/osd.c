/**
 * ISS Tracker
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 */

#include <conio.h>
#include <apple2.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

void osd(char *lon, char *lat,unsigned long ts) {
    uint8_t lon_len;
    uint8_t lat_len;
    lon_len = strlen(lon);
    lat_len = strlen(lat);
    if (lon[lon_len - 1] == 0x0D) lon[lon_len - 1] = '\0';
    if (lat[lat_len - 1] == 0x0D) lat[lat_len - 1] = '\0';

    gotoxy(5, 0);
    cprintf("__ Current ISS Position __");
    gotoxy(2, 1);
    cprintf("Longitude: %-10s",lon);
    gotoxy(2, 2);
    cprintf("Latitude:  %-10s",lat);
    gotoxy(2, 3);
    cprintf("at: %s",ctime(&ts));
}