/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <conio.h>
#include "options.h"
#include "io.h"
#include "location.h"

#include "constants.h"


State state = LOCATION;


void main(void)
{
    int i,j, x=32,y=32;


    memset(locData, 0, sizeof(Location));
    memset(optData, 0, sizeof(optData));
    clrscr();

    cprintf("START 20, %s\n", LOCATION_VERSION);

    io_time();

    csleep(1000);
    cprintf("END\n");

/*
    cprintf("Getting ip\n");
    location_get_from_ip(NULL);
    cprintf("returned from location_get_from_ip\n");
*/
    j=2;
    for(i=0; j!=2; i++)
        csleep(1000);

}