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
#include "weather.h"
#include "icon.h"



static unsigned char response[1024];

/*** JSON ****/


int io_json_open(char *url)
{

    struct _oc
    {
        unsigned char cmd;
        char mode;
        char trans;
        char url[256];
    } OC; // Open command

    struct _scm
    {
        unsigned char cmd;
        char mode;
    } SCM; // Set Channel Mode command

    OC.cmd = 'O';
    OC.mode = 12;
    OC.trans = 3;
    strncpy(OC.url,url,256);

    SCM.cmd = 0xFC;
    SCM.mode = CHANNEL_MODE_JSON;
  
    // open url
    if (eos_write_character_device(NET_DEV,(unsigned char *)OC,sizeof(OC)) != ACK)
    {
#ifdef DISPLAY_DEBUG        
        cprintf("Could not open\n%s\n", OC.url);
        csleep(10000);
#endif
        return 1; // could not open
    }
  
    // set channel to json
    if (eos_write_character_device(NET_DEV,(unsigned char *)SCM,sizeof(SCM)) != ACK)
    {
#ifdef DISPLAY_DEBUG        
        cprintf("Could not set channel to json mode");
        csleep(10000);

#endif
        return 2;  // could not set to json mode
    }
  
    // set parsing mode
    if (eos_write_character_device(NET_DEV,"P",1) != ACK)
    {
#ifdef DISPLAY_DEBUG        
        cprintf("Could not set parsing mode\n");
        csleep(10000);
#endif
        return 3; // could not set to parsing mode
    }

    return 0;
}

 int io_json_query(char *element, char *data, int max_buffer_size)
{
    struct _qcm
    {
        unsigned char cmd;
        char query[128];
    } qcm; // Query command

    unsigned char r = 0;

    memset(response, 0, sizeof(response));

#ifdef DISPLAY_DEBUG        
    cprintf("query: %s\n", element);
#endif

    qcm.cmd = 'Q';
    strcpy(qcm.query, element);
    r = eos_write_character_device(NET_DEV, (unsigned char *)qcm, sizeof(qcm));
    if (r != ACK)
    {
#ifdef DISPLAY_DEBUG        
        cprintf("Did not ack query command\n");
        csleep(10000);
#endif
        return 1; // did not ack query command
    }

    r = eos_read_character_device(NET_DEV, response, sizeof(response));

    memcpy(data, response, max_buffer_size);
    if (r != ACK)
    {
#ifdef DISPLAY_DEBUG        
        cprintf("Did not return response to query\n");
        csleep(10000);
#endif
        return 2; // did not get result
    }

#ifdef DISPLAY_DEBUG        
    cprintf("data: %s\n", data);
#endif
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
        p += decimals;
        *p = '\0';
    }
}
