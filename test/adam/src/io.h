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

#define DISPLAY_DEBUG

#include <stdbool.h>


extern static unsigned char response[1024];

typedef struct {
  unsigned char century,// Century
                year,   // Year
                month,  // Month
                day,    // Day
                hour,   // Hour
                minute, // Minute
                second; // Second
    } FUJI_TIME;


typedef struct {
        unsigned char cmd;
        char mode;
        char trans;
        char url[256];
    } FUJI_CMD; // Open command

typedef struct {
        unsigned char cmd;
        char mode;
    } FUJI_CHANNELMODE; // Set Channel Mode command

typedef struct qcm
    {
        unsigned char cmd;
        char query[128];
    } FUJI_QUERY; // Query command

void io_time(void);

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

#endif /* IO_H */
