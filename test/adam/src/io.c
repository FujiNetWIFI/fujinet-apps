/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <stdbool.h>
#include <eos.h>
#include <string.h>
#include <conio.h>
#include "constants.h"
#include "io.h"
#include "options.h"
#include "ftime.h"
#include "utils.h"




static unsigned char response[1024];

/*** CLOCK ****/

void print_time(FUJI_TIME *ft)
{
    cprintf("TIME:\n");
    cprintf("%2u%02u/%02u/%02u %02u:%02u:%02u\n", (unsigned int) ft->century, (unsigned int) ft->year,(unsigned int) ft->month, (unsigned int) ft->day, (unsigned int) ft->hour, (unsigned int) ft->minute, (unsigned int) ft->second);
}

int io_time(*time)
{
 FUJI_CMD oc;

    unsigned char r = 0;

    oc.cmd = 0xD2; // Get Time

    // request time
    if (eos_write_character_device(FUJI_DEV, (unsigned char *)oc, (unsigned short)sizeof(oc)) != ACK)
    {
        cprintf("Could not open time\n");
        return 1; // could not open
    }

    r = eos_read_character_device(FUJI_DEV, response, (unsigned short)sizeof(response));
    if (r != ACK)
    {
        cprintf("Did not return response to time query\n");
        return 3; // did not get result
    }

    for(r=0; r<sizeof(time); r++)
    {
        cprintf("%02x ", response[r]);
    }
    cprintf("\n");
    memcpy(time, response, sizeof(time));
    return 0;
}

void add_time (FUJI_TIME *result, FUJI_TIME *time1, FUJI_TIME *add_time)
{
    memcpy(result, time1, sizeof(FUJI_TIME));

    result->hour   += add_time->hour;
    result->minute += add_time->minute;
    result->second += add_time->second;

    if (result->second > 59)
    {
        result->minute += 1;
        result->second -= 60;
    }

    if (result->minute > 59)
    {
        result->hour += 1;
        result->minute -= 60;
    }

    if (result->hour > 23)
    {
        result->hour -= 24;
    }

}

unsigned long time_in_seconds(FUJI_TIME *time) 
{ 
    return       time->second + 
          60   * time->minute + 
          3600 * time->hour; 
}

bool wait_for_time(FUJI_TIME *wait_until)
{
static  FUJI_TIME add;
        bool past_time = false;
static  FUJI_TIME  current;
        unsigned long   seconds_current;
        unsigned long   seconds_waiting;


    io_time()
    add_time(&current, &current, &add);
    print_time(&current);

    if (wait_until->hour < current.hour)
    {
        return false;
    }

    seconds_current = time_in_seconds(&current);
    seconds_waiting = time_in_seconds(wait_until);

    return seconds_current >= seconds_waiting;

}


/*** JSON ****/


int io_json_open(char *url)
{
FUJI_CMD oc; // Open command
FUJI_CMD scm; // Set Channel Mode command

    oc.cmd = 'O';
    oc.mode = 12;
    oc.trans = 3;
    strncpy(oc.url,url,sizeof(oc.url));

    scm.cmd = 0xFC;
    scm.mode = CHANNEL_MODE_JSON;
  
    // open url
    if (eos_write_character_device(NET_DEV, (unsigned char *)oc, (unsigned short)sizeof(oc)) != ACK)
    {
        cprintf("Could not open\n%s\n", oc.url);
        return 1; // could not open
    }
  
    // set channel to json
    if (eos_write_character_device(NET_DEV, (unsigned char *)scm, (unsigned short)sizeof(scm)) != ACK)
    {
        cprintf("Could not set channel to json mode");
        return 2;  // could not set to json mode
    }
  
    // set parsing mode
    if (eos_write_character_device(NET_DEV, "P", (unsigned short)1) != ACK)
    {
        cprintf("Could not set parsing mode\n");
        return 3; // could not set to parsing mode
    }

    return 0;
}

 int io_json_query(char *element, char *data, int max_buffer_size)
{
    FUJI_QUERY qcm; // Query command

    unsigned char r = 0;

    memset(response, 0, sizeof(response));

    cprintf("query: %s\n", element);

    qcm.cmd = 'Q';
    strcpy(qcm.query, element);
    r = eos_write_character_device(NET_DEV, (unsigned char *)qcm, (unsigned short ) sizeof(qcm));
    if (r != ACK)
    {
        cprintf("Did not ack query command\n");
        return 1; // did not ack query command
    }
    cprintf("Got ack1\n");
    r = eos_read_character_device(NET_DEV, response, (unsigned short)sizeof(response));

    memcpy(data, response, max_buffer_size);
    data[max_buffer_size-1] = '\0';
    if (r != ACK)
    {
        cprintf("Did not return response to query\n");
        return 2; // did not get result
    }
    cprintf("Got ack2\n");

    cprintf("data: %s\n", data);
    return 0;
}

int io_json_close(void)
{
    eos_write_character_device(NET_DEV, "C", (unsigned short)1);
    return 0;
}

void io_decimals(char *d, int decimals)
{
    char *p = strchr(d, '.');
    if (p)
    {
        p += decimals;
        *p = '\0';
    }
}
