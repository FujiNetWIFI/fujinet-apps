#ifndef GLOBALS_H
#define GLOBALS_H

/* Storage for the current date/time */
unsigned char time_buf[6];
unsigned char cur_yr, cur_mo, cur_day, pick_yr, pick_mo, pick_day;

/* Values for Red, Green, and Blue, to allow hue adjustment */
unsigned char rgb_red, rgb_grn, rgb_blu;

#endif // GLOBALS_H

