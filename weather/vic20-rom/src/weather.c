/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose weather
 */

#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include "defines.h"
#include "icons.h"
#include "video.h"

extern State state;

WeatherData wd;

unsigned char x,y; // Screen coords

void weather_icon_display(const char *s, bool storm)
{
  unsigned char o;
  
  VIC.volume_color=0xF0; // aux = yellow

  o = y*22+x;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[0],4);
  
  o += 22;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[4],4);

  o += 22;
  memset(&COLOR_RAM[o],0x0b,4);
  memcpy(&SCREEN_RAM[o],&s[8],4);

  if (storm)
    {
      o += 22;
      memset(&COLOR_RAM[o],0x0b,4);
      memcpy(&SCREEN_RAM[o],&s[12],4);
    }
}

void weather_test(void)
{
  strcpy(wd.date_txt,"2023-05-29");
  strcpy(wd.sunrise_txt,"6:21 AM");
  strcpy(wd.sunset_txt,"8:30 PM");
  strcpy(wd.time_txt,"10:05 PM");
  strcpy(wd.feels_like,"84@F");
  strcpy(wd.pressure,"1012 hPa");
  strcpy(wd.humidity,"62%");
  strcpy(wd.dew_point,"67%");
  strcpy(wd.clouds,"20%");
  strcpy(wd.visibility,"100%");
  strcpy(wd.wind_speed,"8 mph");
  strcpy(wd.wind_dir,"S");
}

void weather(void)
{
  weather_test();
  clrscr();
  VIC.bg_border_color=0x69;
  memset(COLOR_RAM,0x01,506);


  cbm_k_bsout('\x05'); // white
  
  // Date/time
  gotoxy(1,1);
  print(wd.date_txt);
  print("  ");
  print(wd.time_txt);

  // Icon
  y=3;
  x=8;
  weather_icon_display(i02,false);
  gotoxy(x+4,y);
  print(wd.feels_like);

  gotoxy(0,y+4);

  print(" PRESSURE: ");
  print(wd.pressure);
  print("\n\n");
  
  print(" HUMIDITY: ");
  print(wd.humidity);
  print("\n\n");

  print("   DEW PT: ");
  print(wd.dew_point);
  print("\n\n");

  print("   CLOUDS: ");
  print(wd.clouds);
  print("\n\n");

  print("VISIBILTY: ");
  print(wd.visibility);
  print("\n\n");

  print("     WIND: ");
  print(wd.wind_speed);
  print(" ");
  print(wd.wind_dir);
  print("\n\n");

  print("  SUNRISE: ");
  print(wd.sunrise_txt);
  print("\n\n");

  print("   SUNSET: ");
  print(wd.sunset_txt);

  while(1);
  
}
