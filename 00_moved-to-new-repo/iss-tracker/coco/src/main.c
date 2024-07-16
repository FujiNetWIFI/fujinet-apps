/**
 * @brief   ISS Tracker
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include <cmoc.h>
#include <coco.h>
#include "map.h"
#include "satellite.h"
#include "osd.h"
#include "fetch.h"

int main(void)
{
    while(1)
    {
        map();
        fetch();
        satellite();
        osd();
        sleep(120);
    }
    
    return 0;
}
