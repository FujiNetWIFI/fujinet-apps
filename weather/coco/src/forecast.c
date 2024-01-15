/**
 * @brief FujiNet weather for CoCo
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md, for details.
 * @verbose Forecast Display
 */

#include <cmoc.h>
#include <coco.h>
#include "gfx.h"
#include "thunderstorm.h"
#include "clear.h"
#include "rain.h"

void forecast(int slot,
	      const char *day0,
	      const char *mon0,
	      const char *dow0,
	      const char *lo0,
	      const char *hi0,
	      const char *pressure0,
	      const char *wind0,
	      const char *humidity,
	      unsigned char *icon0)
{
  slot *= 32;

  puts_dbl(slot+4+2,0+2,0,day0);
  puts_dbl(slot+0+2,16+2,0,mon0);
  put_icon(slot+0,32,icon0);
  puts_dbl(slot+0+2,80+2,0,dow0);
  puts(slot+0+2,98+2,2,lo0);
  puts(slot+0+2,108+2,0,hi0);
  puts(slot+0+2,128+2,0,pressure0);
  puts(slot+0+2,140+2,0,wind0);
}

/**
 * @brief Test harness, remove.
 */
int main(void)
{
  initCoCoSupport(0);
  rgb();
  width(32);
  gfx(1);
  gfx_cls(CYAN);

  forecast(0,
	   "31",
	   "Dec",
	   "Sun",
	   "44",
	   "58",
	   "30\x1e\x1f",
	   "\x1d\x31\x35m",
	   "H: 100%",
	   thunderstorm);
  
  forecast(1,
	   "1",
	   "Jan",
	   "Mon",
	   "40",
	   "45",
	   "30\x1e\x1f",
	   "\x1d\x31\x33m",
	   "H: 100%",
	   clear);

  forecast(2,
	   "2",
	   "Jan",
	   "Tue",
	   "43",
	   "47",
	   "30\x1e\x1f",
	   "\x1d\x31\x33m",
	   "H: 100%",
	   rain);

  forecast(3,
	   "3",
	   "Jan",
	   "Wed",
	   "47",
	   "50",
	   "30\x1e\x1f",
	   "\x1d\x31\x33m",
	   "H: 100%",
	   rain);

  while(1);

  return 0;
}
