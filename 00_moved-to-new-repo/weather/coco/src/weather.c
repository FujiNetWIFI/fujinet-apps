/**
 * @brief FujiNet weather for CoCo
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md, for details.
 * @verbose Weather display
 */

#include <cmoc.h>
#include <coco.h>
#include "gfx.h"

void weather(const char *year, const char *month, const char *day,
	     const char *hour, const char *min,
	     const char *region,
	     const char *temp,
	     const char *condition,
	     const char *pressure,
	     const char *humidity,
	     const char *dew_pt,
	     const char *clouds,
	     const char *visibility,
	     const char *wind,
	     const char *sunrise,
	     const char *sunset,
	     unsigned char *icon)
{
  gfx_cls(CYAN);

  // 2023-12-31 18:25
  puts(0,4,2,year);
  puts(32,4,2,"-");
  puts(40,4,2,month);
  puts(56,4,2,"-");
  puts(64,4,2,day);
  puts(88,4,2,hour);
  puts(104,4,2,":");
  puts(112,4,2,min);

  // weather conditions icon
  put_icon(8,16,icon);

  // Temperature
  puts_dbl(40,24,0,temp);

  // Pressure
  puts_dbl(40,40,0,pressure);

  // Condition
  puts(0,72,0,condition);

  // Region
  puts(0,88,0,region);

  // Humidity
  puts(0+4,104,0,"HUMIDITY:");
  puts(88+4,104,2,humidity);

  // Dew Point
  puts(0+4,116,0,"DEW PT:");
  puts(64+4,116,2,dew_pt);

  // Clouds
  puts(0+4,128,0,"CLOUDS:");
  puts(88+4,128,2,clouds);

  // Visibility
  puts(0+4,140,0,"VISBLITY:");
  puts(88+4,140,2,visibility);

  // Wind
  puts(0+4,152,0,"W:");
  puts(24+4,152,2,wind);

  // Sunrise
  puts(0+4,164,0,"SUNRISE:");
  puts(64+4,164,2,sunrise);

  // Sunset
  puts(0+4+8,176,0,"SUNSET:");
  puts(64+4,176,2,sunset);

}

/**
 * @brief Test harness, remove.
 */
/* int main(void) */
/* { */
/*   initCoCoSupport(0); */
/*   rgb(); */
/*   width(32); */
/*   gfx(1); */
  
/*   weather("2023","12","31", */
/* 	  "18","25", */
/* 	  "   Denton, US   ", */
/* 	  "53.0*F", */
/* 	  "  thunderstorm  ", */
/* 	  "30.60 \"Hg", */
/* 	  " 61%", */
/* 	  "39.99*F", */
/* 	  "  0%", */
/* 	  "10km", */
/* 	  " 6.91 mph NE", */
/* 	  " 7:31AM", */
/* 	  " 5:30PM", */
/* 	  thunderstorm); */

/*   while(1); */
  
/*   return 0; */
/* } */
