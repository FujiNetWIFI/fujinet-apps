/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose forecast
 */

#include <stdbool.h>
#include <vic20.h>
#include <cbm.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "location.h"
#include "icons.h"
#include "video.h"
#include "color.h"

extern unsigned char x,y;
extern void weather_icon_display(const char *s, bool storm);
extern void weather_test(void);
extern WeatherData wd[3];
extern Location l;
extern unsigned short wait;
extern State state;
extern Unit units;

void forecast_test(void)
{
  weather_test();
  strcpy(wd[0].date_txt,"05-29");
  strcpy(wd[1].date_txt,"05-30");
  strcpy(wd[2].date_txt,"05-31");
  strcpy(wd[0].temp_lo,"71.96");
  strcpy(wd[1].temp_lo,"75.61");
  strcpy(wd[2].temp_lo,"76.53");
  strcpy(wd[0].temp_hi,"81.34");
  strcpy(wd[1].temp_hi,"83.10");
  strcpy(wd[2].temp_hi,"84.81");
  strcpy(wd[0].pressure,"1013\x12\x62\x63\x92");
  strcpy(wd[1].pressure,"1012\x12\x62\x63\x92");
  strcpy(wd[2].pressure,"1011\x12\x62\x63\x92");
  strcpy(wd[0].wind_dir,"SE");
  strcpy(wd[1].wind_dir,"SE");
  strcpy(wd[2].wind_dir,"SE");
  strcpy(wd[0].wind_speed,"19.08");
  strcpy(wd[1].wind_speed,"10.12");
  strcpy(wd[2].wind_speed,"14.12");
  strcpy(wd[0].humidity,"\x12\x64\x92""70%");
  strcpy(wd[1].humidity,"\x12\x64\x92""67%");
  strcpy(wd[2].humidity,"\x12\x64\x92""64%");
}

void forecast(void)
{
  forecast_test();
  clrscr();
  color_setup();

  y=3;

  x=1;
  weather_icon_display(i01,false);

  x=8;
  weather_icon_display(i02,false);

  x=16;
  weather_icon_display(i03,false);

  gotoxy(1,y-2);

  print(wd[0].date_txt);
  print("  ");
  print(wd[1].date_txt);
  print("  ");
  print(wd[2].date_txt);

  gotoxy(1,y+4);
  print("\x9C");
  print(wd[0].temp_lo);
  print("  ");
  print(wd[1].temp_lo);  
  print("  ");
  print(wd[2].temp_lo);

  print("\x1E\n");
  
  print(" ");
  print(wd[0].temp_hi);
  print("  ");
  print(wd[1].temp_hi);  
  print("  ");
  print(wd[2].temp_hi);

  print("\x05\n");

  gotoxy(1,y+7);
  print(wd[0].pressure);
  gotoxy(8,y+7);
  print(wd[1].pressure);
  gotoxy(15,y+7);
  print(wd[2].pressure);
  print("\n\n");

  gotoxy(1,y+9);
  print("W: ");
  print(wd[0].wind_dir);
  gotoxy(8,y+9);
  print("W: ");
  print(wd[1].wind_dir);
  gotoxy(15,y+9);
  print("W: ");
  print(wd[2].wind_dir);

  gotoxy(1,y+10);
  print(wd[0].wind_speed);
  gotoxy(8,y+10);
  print(wd[1].wind_speed);
  gotoxy(15,y+10);
  print(wd[2].wind_speed);

  gotoxy(1,y+12);
  print(wd[0].humidity);
  gotoxy(8,y+12);
  print(wd[1].humidity);
  gotoxy(15,y+12);
  print(wd[2].humidity);
  
  gotoxy(1,17);
  print(l.city);

  gotoxy(1,18);
  print(l.region_code);
  cbm_k_bsout(' ');
  print(l.country_code);

  gotoxy(0,21);
  print(" \x12""F1\x92 TODAY    \x12""F3\x92 UNITS");

  wait=(unsigned short)WAIT_TIME;

  while (wait--)
    {
      if (kbhit())
	{
	  switch(cgetc())
	    {
	    case CH_F1:
	      state=WEATHER;
	      return;
	    case CH_F2:
	      units^=units;
	      return;
	    }
	}
      waitvsync();
    }
}
