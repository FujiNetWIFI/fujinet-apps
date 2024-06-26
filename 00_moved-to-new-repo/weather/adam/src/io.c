/**
 * Weather / io.c
 *
 * Based on @bocianu's code
 *
 * @author Norman Davie
 *
 */

#include <stdbool.h>
#include <eos.h>
#include <string.h>
#include <conio.h>
#include "constants.h"
#include "io.h"
#include "ftime.h"
#include "utils.h"
#include "weather.h"



unsigned char response[1024];

char *strncpy2(char *dest, char *src, size_t size)
{
    char *p;

    p = strncpy(dest, src, size-1);
    dest[size-1] = '\0';
    return p;
}

char *io_get_next_string(char *start, char *result, int max_size)
{
    char *end;
    char *p;
    int i = 0;

    if (start == NULL)
        return NULL;

    end = start;
    if ((*end == '\0') || (*end == '\n'))
        return NULL;

    while (! ((*end == '|') || (*end == '\n') || (*end == '\0')))
    {
        if (i >= max_size-1)
            break;

        *result = *end;
        result++;
        end++;
        i++;
    }
    *result = '\0';
    
    if ((*end == '|') || (*end == '\n'))
        end++;

    return end;
}

char *io_get_next_bool(char *start, bool *result)
{
    char temp[10];

    if (start == NULL)
        return NULL;

    char *p = io_get_next_string(start, temp, sizeof(temp));
    *result = (bool) (atoi(temp) != 0);
    return p;
}

char *io_get_next_int(char *start, int *result)
{
    char temp[10];

    if (start == NULL)
    {
        *result = 0;
        return NULL;
    }
    char *p = io_get_next_string(start, temp, sizeof(temp));
    *result = atoi(temp);
    return p;
}

char *io_get_next_long(char *start, long *result)
{
    char temp[10];

    if (start == NULL)
    {
        *result = 0L;
        return NULL;
    }   
    char *p = io_get_next_string(start, temp, sizeof(temp));
    *result = atol(temp);
    return p;
}


void debug_print(char *s)
{
    if (s == NULL)
        cprintf("NULL\n");
    else
        cprintf("%s\n",s);
    csleep(DEBUG_DELAY);
}

/*** CLOCK ****/

int io_time(FUJI_TIME *time)
{
 FUJI_CMD oc;

    unsigned char r = 0;

    oc.cmd = 0xD2; // Get Time

    // request time
    if (eos_write_character_device(FUJI_DEV, (unsigned char *)oc, (unsigned short)sizeof(oc)) != ACK)
    {
        return 1; // could not open
    }

    r = eos_read_character_device(FUJI_DEV, response, (unsigned short)sizeof(response));
    if (r != ACK)
    {
        return 3; // did not get result
    }

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
        result->minute += (result->second / 60);
        result->second %= 60;
    }

    if (result->minute > 59)
    {
        result->hour += (result->minute / 60);
        result->minute %= 60;
    }

    if (result->hour > 23)
    {
        result->hour %= 24;
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
    FUJI_TIME  current;
    bool past_time = false;
    unsigned long   seconds_current;
    unsigned long   seconds_waiting;


    io_time(&current);

    if (wait_until->hour < current.hour)
        return false;

    seconds_current = time_in_seconds(&current);
    seconds_waiting = time_in_seconds(wait_until);

    return seconds_current >= seconds_waiting;

}



/*** JSON ****/


int io_json_open(char *url)
{
    FUJI_CMD oc;
    FUJI_SET_CHANNEL scm;

    oc.cmd = 'O';
    oc.mode = 12;
    oc.trans = 3;
    strncpy2(oc.url,url,sizeof(oc.url));

    scm.cmd = 0xFC;
    scm.mode = CHANNEL_MODE_JSON;

    // open url
    if (eos_write_character_device(NET_DEV,(unsigned char *)oc,sizeof(oc)) != ACK)
    {
        return 1; // could not open
    }
  
    // set channel to json
    if (eos_write_character_device(NET_DEV,(unsigned char *)scm,sizeof(scm)) != ACK)
    {
        return 2;  // could not set to json mode
    }
  
    // set json parsing mode
    if (eos_write_character_device(NET_DEV,"P",1) != ACK)
    {
        return 3; // could not set to parsing mode
    }

    return 0;
}

 int io_json_query(char *element, char *data, int max_buffer_size)
{
    FUJI_JSON_QUERY qcm;
    unsigned char r = 0;

    // zero out the response buffer
    memset(response, 0, sizeof(response));

    qcm.cmd = 'Q';
    strcpy(qcm.query, element);
    r = eos_write_character_device(NET_DEV, (unsigned char *)qcm, sizeof(qcm));
    if (r != ACK)
    {
        return 1; // did not ack query command
    }

    r = eos_read_character_device(NET_DEV, response, sizeof(response));
    if (r != ACK)
    {
        return 2; // did not get result
    }

    response[max_buffer_size-1] = '\0';
    memcpy(data, response, max_buffer_size);
    return 0;
}

int io_json_close(void)
{
    eos_write_character_device(NET_DEV,"C",1); 
    return 0;
}

void io_decimals(char *d, int decimals)
{
    char *p = strchr(d, '.');
    if (p)
    {
        p += (decimals + 1);
        *p = '\0';
    }
}
