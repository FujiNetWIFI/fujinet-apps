#ifndef GET_TIME_H
#define GET_TIME_H

extern unsigned char cur_yr, cur_mo, cur_day, pick_yr, pick_mo, pick_day;
extern unsigned char last_day[13];
extern unsigned char time_buf[6];

void get_time(void);

#endif // GET_TIME_H
