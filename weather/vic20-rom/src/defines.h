/**
 * @brief   Weather app for VIC-20 cartridge
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 * @verbose defines
 */

#ifndef DEFINES_H
#define DEFINES_H

typedef enum _state
{
  LOCATION,
  WEATHER,
  EXIT
} State;

typedef struct _weatherData
{
  char date_txt[32];
  char sunrise_txt[16];
  char sunset_txt[16];
  char time_txt[16];
  char temp[24];
  char timezone[16];
  char feels_like[16];
  unsigned short timezone_offset;
  char pressure[14];
  char humidity[16];
  char dew_point[16];
  char clouds[16];
  char visibility[16];
  char wind_speed[16];
char wind_dir[3];
  char wind_txt[16];
  char description[24];
  char loc[48];
} WeatherData;

#define LFN 2
#define DEV 12
#define SAN 2
#define CMD 15

#define I_SIZE 12 // normal sized icon
#define S_SIZE 16 // storm sized icon

#define SCREEN_RAM ((unsigned char *)0x1E00)

#define IP_API "http://api.ipstack.com/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude"

#endif /* DEFINES_H */
