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

typedef enum _unit
  {
    IMPERIAL,
    METRIC
  } Unit;

typedef struct _weatherData
{
  char date_txt[22];
  char sunrise_txt[10];
  char sunset_txt[10];
  char time_txt[10];
  char feels_like[12];
  char temp_hi[10];
  char temp_lo[10];
  char pressure[10];
  char humidity[10];
  char dew_point[10];
  char clouds[5];
  char visibility[7];
  char wind_speed[16];
  char wind_dir[4];
  char icon[4];
  char description[20];
} WeatherData;

#define LFN 2
#define DEV 16    // The network device #
#define SAN 2
#define CMD 15

#define I_SIZE 12 // normal sized icon
#define S_SIZE 16 // storm sized icon

#define SCREEN_RAM ((unsigned char *)0x1E00)

#define IP_API "http://api.ipstack.com/check?access_key=d05a249425c32e0b231ba79e3f9af478&fields=ip,region_code,country_code,city,latitude,longitude"

#define WAIT_TIME 65533 // vblanks

#endif /* DEFINES_H */
