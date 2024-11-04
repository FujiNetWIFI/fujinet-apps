#ifdef _COCO_BASIC_
#include <cmoc.h>
#include <coco.h>
#else
#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#endif /* COCO_BASIC */
#include "fetch.h"
#include "parse.h"
#include "screen_election.h"
#include "ruminate.h"

#define FOREVER 1

const char *version = "1.0.0";

int main() 
{
    screen_init();

    while(FOREVER)
    {
        fetch();
        parse(s);
        screen_election();
        ruminate();
    }

    return 0;
}
