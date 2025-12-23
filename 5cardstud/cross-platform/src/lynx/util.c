#include <lynx.h>
#include <tgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


unsigned long start_time;


void resetTimer()
{
    start_time = clock();
}

int getTime()
{
    unsigned long now_time;
    
    now_time = clock();
    return((int) (now_time - start_time));
}

void quit()
{
    while(1);
}