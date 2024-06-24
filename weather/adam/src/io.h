/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef IO_H
#define IO_H

#include <stdbool.h>

#define DISPLAY_DEBUG 1
#define DEBUG_DELAY (400)

#define MAX_APP_DATA (128)
#define MAX_URL      (256)
#define MAX_QUERY    (128)

extern unsigned char response[1024];



typedef struct
{
     unsigned char century, // Century
         year,              // Year
         month,             // Month
         day,               // Day
         hour,              // Hour
         minute,            // Minute
         second;            // Second
} FUJI_TIME;

typedef struct
{
    unsigned char cmd;
    unsigned short mode;
    unsigned char trans;
    unsigned char url[MAX_URL];
} FUJI_CMD;

typedef struct 
{
  unsigned char cmd;
           char mode
} FUJI_SET_CHANNEL;


typedef struct 
{
  unsigned char cmd;
  char query[MAX_QUERY];
} FUJI_JSON_QUERY;


typedef struct
{
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
} FUJI_APP;

 typedef struct
{
    unsigned char cmd;
    unsigned short creator;
    unsigned char app;
    unsigned char key;
    char data[MAX_APP_DATA];
} FUJI_APP_DATA;

char *strncpy2(char *dest, char *src, size_t size);
void print_time(FUJI_TIME *ft);

/*
io_time 
- This function gets the time from the fujinet device
Parameters
wait_until: FUJI_TIME representing the time to wait for.

Returns
    0: Success
    1: Could not open website
    3: Could not get result
*/
int io_time(FUJI_TIME *time);

void add_time (FUJI_TIME *result, FUJI_TIME *time1, FUJI_TIME *add_time);

/*
wait_for_time 
- This function will return true if the supplied time has exceeded
Parameters
wait_until: FUJI_TIME representing the time to wait for.

Returns
 true: current time is equal or exceeded 'wait_util'
*/
bool wait_for_time(FUJI_TIME *wait_until);

/*
io_json_open 
- This function will open the website, switch to json mode and prepare for parsing
  After calling this function you may use io_json_query

Parameters
  url: website to request the json information

Returns
    0: Success
    1: Could not open website
    2: Could not switch to json mode
    3: Could not set parsing mode
*/
 int io_json_open(char *url);

/*
io_json_query 
- After performing a io_json_open, request the json element

Parameters
- element:          the json element to collect
- data:             the data within the element as a string
                    if unsuccessful, data buffer will be an empty string
- max_buffer_size:  the maximum size of the string including the null terminator

Returns
    0: Success
    1: Did not receive acknowledgment of the query command
    2: Could not find the element
 
*/
 int io_json_query(char *element, char *data, int max_buffer_size);



/*
io_json_close 
- Close the website used by the last io_json_open.  No check is performed.

Returns
    0: Success
 
*/ 
 int io_json_close(void);

/*
io_decimals 
- This function modifies the contents of the string buffer number
  by placing a null character two places after a ',' is found,
  otherwise the contents remain unchanged.

Parameters
  number: the string representing the floating point number

*/
 void io_decimals(char *number, int decimals);
 void debug_print(char *s);
 char *io_get_next_string(char *start, char *result, int max_size);
 char *io_get_next_int(char *start, int *result);
 char *io_get_next_long(char *start, long *result);
 char *io_get_next_bool(char *start, bool *result);


#endif /* IO_H */
