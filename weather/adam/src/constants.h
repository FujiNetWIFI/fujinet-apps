/**
 * Weather
 * 
 * Constants
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define APPKEY_CREATOR_ID 0xb0c1
#define APPKEY_APP_ID 0x1
#define APPKEY_CONFIG_KEY 0xC0
#define APPKEY_LOCATION_KEY 0x10

#define DEFAULT_REFRESH 120

#define IP_API "api.ipstack.com"
#define OW_API "api.openweathermap.org"
#define OC_API "api.opencagedata.com"

#define ICON_CLEAR_SKY        0
#define ICON_FEW_CLOUDS       1
#define ICON_SCATTERED_CLOUDS 2
#define ICON_BROKEN_CLOUDS    3
#define ICON_SHOWER_RAIN      4
#define ICON_RAIN             5
#define ICON_THUNDERSTORM     6
#define ICON_SNOW             7
#define ICON_MIST             8

typedef enum _state
  {
   WEATHER,
   FORECAST,
   LOCATION,
   DONE
  } State;

#endif /* CONSTANTS_H */
