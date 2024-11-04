/**
 * @brief screen routines for coco
 */

#include <coco.h>
#include "globals.h"

#define COLOR_REP 0xBF
#define COLOR_DEM 0xAF

void screen_init(void)
{
}

void screen_display_field(char *l, int d, int c, int sh, int h)
{
    int i=0, j=0;
    
    printf("%-29s%3d",l,d);
    for (i=0;i<h;i++)
    {
        if (d)
        {
            int count = d >> sh;

            if (count>31)
                count=31; /* prevent spill */
            
            for (j=0;j<count;j++)
            {
                putchar(c);
            }
            putchar('\n');
        }
        else
            putchar('\n');
    }
}

void screen_election(void)
{
    int i=0;
    
    cls(5);

    printf("#fujinet\x80");
    printf("election\x80\x80\x80");
    printf("2024\x80results");
    screen_display_field(l2,d2,COLOR_REP,3,1);
    screen_display_field(l1,d1,COLOR_DEM,3,1);
    printf("\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80senate\x80seats\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80");
    screen_display_field((char *)"republicans",d4,COLOR_REP,2,1);
    screen_display_field((char *)"democrats",d3,COLOR_DEM,2,1);
    printf("\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80reps\x80seats\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80");
    screen_display_field((char *)"republicans",d6,COLOR_REP,5,1);
    screen_display_field((char *)"democrats",d5,COLOR_DEM,5,1);
    printf("%-16s%15s",d,t);

    if (d2>269)
    {
        locate(16,1);
        printf("WINS!");
    }
    else if (d1>269)
    {
        locate(16,3);
        printf("WINS");
    }
}
