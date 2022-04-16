#include <stdio.h>
#include <string.h>
#include "faux_json.h"

/*
Compile with:

gcc json_test.c faux_json.c -o json_test && ./json_test

Or, for some debugging:

gcc -D HOST json_test.c faux_json.c -o json_test && ./json_test
*/

int main(int argc, char * argv[]) {
  /* TEST 1 */
  strcpy(json,
    "{\"timestamp\": 1621321797, \"iss_position\": {\"longitude\": \"-91.0060\", \"latitude\": \"46.9216\"}, \"message\": \"success\"}"
  );
  printf("Checking LONGITUDE from\n%s\n", json);
  faux_parse_json("\"longitude\": ", 0);
  printf("json part = <<<%s>>>\n\n", json_part);


  strcpy(json,
    "{\"number\": 7, \"message\": \"success\", \"people\": [{\"name\": \"Mark Vande Hei\", \"craft\": \"ISS\"}, {\"name\": \"Oleg Novitskiy\", \"craft\": \"ISS\"}, {\"name\": \"Pyotr Dubrov\", \"craft\": \"ISS\"}, {\"name\": \"Thomas Pesquet\", \"craft\": \"ISS\"}, {\"name\": \"Megan McArthur\", \"craft\": \"ISS\"}, {\"name\": \"Shane Kimbrough\", \"craft\": \"ISS\"}, {\"name\": \"Akihiko Hoshide\", \"craft\": \"ISS\"}]}"
  );

  /* TEST 2 */
  printf("Checking NUMBER from\n%s\n", json);
  faux_parse_json("\"number\": ", 0);
  printf("json part = <<<%s>>>\n\n", json_part);

  /* TEST 3 */
  printf("Checking PEOPLE.NAME[0] from\n%s\n", json);
  faux_parse_json("\"name\": ", 0);
  printf("json part = <<<%s>>>\n\n", json_part);

  /* TEST 4 */
  printf("Checking PEOPLE.NAME[1] from\n%s\n", json);
  faux_parse_json("\"name\": ", 1);
  printf("json part = <<<%s>>>\n\n", json_part);


  /* TEST 5 (https://openweathermap.org/api/one-call-api) */
  /* Example output:
     {
       "lat": 33.1451,
       "lon": -97.088,
       "timezone": "America/Chicago",
       "timezone_offset": -21600,
       "daily": [
         {
           "dt": 1646589600,
           "sunrise": 1646571023,
           "sunset": 1646612946,
           "moonrise": 1646579460,
           "moonset": 1646628000,
           "moon_phase": 0.14,
           "temp": {
             "day": 69.53,
             "min": 55.06,
             "max": 72.59,
             "night": 55.06,
             "eve": 69.8,
             "morn": 64.94
           },
           "feels_like": {
             "day": 69.89,
             "night": 54.39,
             "eve": 70.2,
             "morn": 65.37
           },
           "pressure": 1012,
           "humidity": 79,
           "dew_point": 62.73,
           "wind_speed": 20.04,
           "wind_deg": 181,
           "wind_gust": 31.59,
           "weather": [
             {
               "id": 501,
               "main": "Rain",
               "description": "moderate rain",
               "icon": "10d"
             }
           ],
           "clouds": 100,
           "pop": 1,
           "rain": 4.68,
           "uvi": 1.91
         },
         {
           "dt": 1646676000,
           "sunrise": 1646657347,
           "sunset": 1646699393,
           "moonrise": 1646667660,
           "moonset": 1646717940,
           "moon_phase": 0.17,
           "temp": {
             "day": 48.9,
             "min": 42.33,
             "max": 53.29,
             "night": 42.82,
             "eve": 49.12,
             "morn": 43.3
           },
           "feels_like": {
             "day": 42.57,
             "night": 37.83,
             "eve": 44.42,
             "morn": 34.86
           },
           "pressure": 1026,
           "humidity": 44,
           "dew_point": 27.61,
           "wind_speed": 19.89,
           "wind_deg": 5,
           "wind_gust": 32.86,
           "weather": [
             {
               "id": 803,
               "main": "Clouds",
               "description": "broken clouds",
               "icon": "04d"
             }
           ],
           "clouds": 72,
           "pop": 0.8,
           "uvi": 6.39
         },
         {
           "dt": 1646762400,
           "sunrise": 1646743670,
           "sunset": 1646785839,
           "moonrise": 1646756040,
           "moonset": 0,
           "moon_phase": 0.2,
           "temp": {
             "day": 50.49,
             "min": 38.37,
             "max": 54.59,
             "night": 46.11,
             "eve": 52.14,
             "morn": 38.68
           },
           "feels_like": {
             "day": 47.5,
             "night": 43.07,
             "eve": 49.5,
             "morn": 33.69
           },
           "pressure": 1022,
           "humidity": 48,
           "dew_point": 31.57,
           "wind_speed": 10.94,
           "wind_deg": 54,
           "wind_gust": 21.59,
           "weather": [
             {
               "id": 804,
               "main": "Clouds",
               "description": "overcast clouds",
               "icon": "04d"
             }
           ],
           "clouds": 100,
           "pop": 0,
           "uvi": 5.82
         },
         {
           "dt": 1646848800,
           "sunrise": 1646829993,
           "sunset": 1646872285,
           "moonrise": 1646844600,
           "moonset": 1646807820,
           "moon_phase": 0.23,
           "temp": {
             "day": 53.82,
             "min": 38.7,
             "max": 59.02,
             "night": 50.86,
             "eve": 56.44,
             "morn": 38.7
           },
           "feels_like": {
             "day": 51.01,
             "night": 48.38,
             "eve": 54,
             "morn": 34.72
           },
           "pressure": 1017,
           "humidity": 45,
           "dew_point": 33.13,
           "wind_speed": 8.68,
           "wind_deg": 150,
           "wind_gust": 15.7,
           "weather": [
             {
               "id": 804,
               "main": "Clouds",
               "description": "overcast clouds",
               "icon": "04d"
             }
           ],
           "clouds": 97,
           "pop": 0,
           "uvi": 5.35
         },
         {
           "dt": 1646935200,
           "sunrise": 1646916315,
           "sunset": 1646958730,
           "moonrise": 1646933520,
           "moonset": 1646897700,
           "moon_phase": 0.25,
           "temp": {
             "day": 64.49,
             "min": 42.21,
             "max": 64.49,
             "night": 42.21,
             "eve": 49.84,
             "morn": 44.83
           },
           "feels_like": {
             "day": 62.8,
             "night": 32.5,
             "eve": 43.23,
             "morn": 40.01
           },
           "pressure": 1009,
           "humidity": 46,
           "dew_point": 43.39,
           "wind_speed": 24.11,
           "wind_deg": 356,
           "wind_gust": 33.96,
           "weather": [
             {
               "id": 800,
               "main": "Clear",
               "description": "clear sky",
               "icon": "01d"
             }
           ],
           "clouds": 1,
           "pop": 0,
           "uvi": 6
         },
         {
           "dt": 1647021600,
           "sunrise": 1647002637,
           "sunset": 1647045176,
           "moonrise": 1647022740,
           "moonset": 1646987340,
           "moon_phase": 0.29,
           "temp": {
             "day": 46.09,
             "min": 30.33,
             "max": 51.55,
             "night": 45.1,
             "eve": 49.3,
             "morn": 30.33
           },
           "feels_like": {
             "day": 40.15,
             "night": 41.76,
             "eve": 47.59,
             "morn": 20.25
           },
           "pressure": 1028,
           "humidity": 27,
           "dew_point": 13.6,
           "wind_speed": 20,
           "wind_deg": 1,
           "wind_gust": 34.2,
           "weather": [
             {
               "id": 802,
               "main": "Clouds",
               "description": "scattered clouds",
               "icon": "03d"
             }
           ],
           "clouds": 45,
           "pop": 0,
           "uvi": 6
         },
         {
           "dt": 1647108000,
           "sunrise": 1647088959,
           "sunset": 1647131621,
           "moonrise": 1647112260,
           "moonset": 1647076860,
           "moon_phase": 0.32,
           "temp": {
             "day": 56.95,
             "min": 39.69,
             "max": 62.15,
             "night": 53.49,
             "eve": 57.56,
             "morn": 39.69
           },
           "feels_like": {
             "day": 53.42,
             "night": 50.29,
             "eve": 54.37,
             "morn": 36.48
           },
           "pressure": 1027,
           "humidity": 23,
           "dew_point": 19.33,
           "wind_speed": 19.33,
           "wind_deg": 187,
           "wind_gust": 35.63,
           "weather": [
             {
               "id": 804,
               "main": "Clouds",
               "description": "overcast clouds",
               "icon": "04d"
             }
           ],
           "clouds": 92,
           "pop": 0,
           "uvi": 6
         },
         {
           "dt": 1647194400,
           "sunrise": 1647175280,
           "sunset": 1647218066,
           "moonrise": 1647202080,
           "moonset": 1647169680,
           "moon_phase": 0.35,
           "temp": {
             "day": 62.04,
             "min": 45.63,
             "max": 67.1,
             "night": 59.99,
             "eve": 64.09,
             "morn": 45.63
           },
           "feels_like": {
             "day": 60.35,
             "night": 58.75,
             "eve": 62.69,
             "morn": 38.62
           },
           "pressure": 1021,
           "humidity": 51,
           "dew_point": 43.56,
           "wind_speed": 23.87,
           "wind_deg": 188,
           "wind_gust": 39.08,
           "weather": [
             {
               "id": 802,
               "main": "Clouds",
               "description": "scattered clouds",
               "icon": "03d"
             }
           ],
           "clouds": 35,
           "pop": 0,
           "uvi": 6
         }
       ]
     }
  */
  {
  strncpy(json,
    "{\"lat\":33.1451,\"lon\":-97.088,\"timezone\":\"America/Chicago\",\"timezone_offset\":-21600,\"daily\":[{\"dt\":1646589600,\"sunrise\":1646571023,\"sunset\":1646612946,\"moonrise\":1646579460,\"moonset\":1646628000,\"moon_phase\":0.14,\"temp\":{\"day\":69.53,\"min\":55.06,\"max\":72.59,\"night\":55.06,\"eve\":69.8,\"morn\":64.94},\"feels_like\":{\"day\":69.89,\"night\":54.39,\"eve\":70.2,\"morn\":65.37},\"pressure\":1012,\"humidity\":79,\"dew_point\":62.73,\"wind_speed\":20.04,\"wind_deg\":181,\"wind_gust\":31.59,\"weather\":[{\"id\":501,\"main\":\"Rain\",\"description\":\"moderate rain\",\"icon\":\"10d\"}],\"clouds\":100,\"pop\":1,\"rain\":4.68,\"uvi\":1.91},{\"dt\":1646676000,\"sunrise\":1646657347,\"sunset\":1646699393,\"moonrise\":1646667660,\"moonset\":1646717940,\"moon_phase\":0.17,\"temp\":{\"day\":48.9,\"min\":42.33,\"max\":53.29,\"night\":42.82,\"eve\":49.12,\"morn\":43.3},\"feels_like\":{\"day\":42.57,\"night\":37.83,\"eve\":44.42,\"morn\":34.86},\"pressure\":1026,\"humidity\":44,\"dew_point\":27.61,\"wind_speed\":19.89,\"wind_deg\":5,\"wind_gust\":32.86,\"weather\":[{\"id\":803,\"main\":\"Clouds\",\"description\":\"broken clouds\",\"icon\":\"04d\"}],\"clouds\":72,\"pop\":0.8,\"uvi\":6.39},{\"dt\":1646762400,\"sunrise\":1646743670,\"sunset\":1646785839,\"moonrise\":1646756040,\"moonset\":0,\"moon_phase\":0.2,\"temp\":{\"day\":50.49,\"min\":38.37,\"max\":54.59,\"night\":46.11,\"eve\":52.14,\"morn\":38.68},\"feels_like\":{\"day\":47.5,\"night\":43.07,\"eve\":49.5,\"morn\":33.69},\"pressure\":1022,\"humidity\":48,\"dew_point\":31.57,\"wind_speed\":10.94,\"wind_deg\":54,\"wind_gust\":21.59,\"weather\":[{\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast clouds\",\"icon\":\"04d\"}],\"clouds\":100,\"pop\":0,\"uvi\":5.82},{\"dt\":1646848800,\"sunrise\":1646829993,\"sunset\":1646872285,\"moonrise\":1646844600,\"moonset\":1646807820,\"moon_phase\":0.23,\"temp\":{\"day\":53.82,\"min\":38.7,\"max\":59.02,\"night\":50.86,\"eve\":56.44,\"morn\":38.7},\"feels_like\":{\"day\":51.01,\"night\":48.38,\"eve\":54,\"morn\":34.72},\"pressure\":1017,\"humidity\":45,\"dew_point\":33.13,\"wind_speed\":8.68,\"wind_deg\":150,\"wind_gust\":15.7,\"weather\":[{\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast clouds\",\"icon\":\"04d\"}],\"clouds\":97,\"pop\":0,\"uvi\":5.35},{\"dt\":1646935200,\"sunrise\":1646916315,\"sunset\":1646958730,\"moonrise\":1646933520,\"moonset\":1646897700,\"moon_phase\":0.25,\"temp\":{\"day\":64.49,\"min\":42.21,\"max\":64.49,\"night\":42.21,\"eve\":49.84,\"morn\":44.83},\"feels_like\":{\"day\":62.8,\"night\":32.5,\"eve\":43.23,\"morn\":40.01},\"pressure\":1009,\"humidity\":46,\"dew_point\":43.39,\"wind_speed\":24.11,\"wind_deg\":356,\"wind_gust\":33.96,\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01d\"}],\"clouds\":1,\"pop\":0,\"uvi\":6},{\"dt\":1647021600,\"sunrise\":1647002637,\"sunset\":1647045176,\"moonrise\":1647022740,\"moonset\":1646987340,\"moon_phase\":0.29,\"temp\":{\"day\":46.09,\"min\":30.33,\"max\":51.55,\"night\":45.1,\"eve\":49.3,\"morn\":30.33},\"feels_like\":{\"day\":40.15,\"night\":41.76,\"eve\":47.59,\"morn\":20.25},\"pressure\":1028,\"humidity\":27,\"dew_point\":13.6,\"wind_speed\":20,\"wind_deg\":1,\"wind_gust\":34.2,\"weather\":[{\"id\":802,\"main\":\"Clouds\",\"description\":\"scattered clouds\",\"icon\":\"03d\"}],\"clouds\":45,\"pop\":0,\"uvi\":6},{\"dt\":1647108000,\"sunrise\":1647088959,\"sunset\":1647131621,\"moonrise\":1647112260,\"moonset\":1647076860,\"moon_phase\":0.32,\"temp\":{\"day\":56.95,\"min\":39.69,\"max\":62.15,\"night\":53.49,\"eve\":57.56,\"morn\":39.69},\"feels_like\":{\"day\":53.42,\"night\":50.29,\"eve\":54.37,\"morn\":36.48},\"pressure\":1027,\"humidity\":23,\"dew_point\":19.33,\"wind_speed\":19.33,\"wind_deg\":187,\"wind_gust\":35.63,\"weather\":[{\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast clouds\",\"icon\":\"04d\"}],\"clouds\":92,\"pop\":0,\"uvi\":6},{\"dt\":1647194400,\"sunrise\":1647175280,\"sunset\":1647218066,\"moonrise\":1647202080,\"moonset\":1647169680,\"moon_phase\":0.35,\"temp\":{\"day\":62.04,\"min\":45.63,\"max\":67.1,\"night\":59.99,\"eve\":64.09,\"morn\":45.63},\"feels_like\":{\"day\":60.35,\"night\":58.75,\"eve\":62.69,\"morn\":38.62},\"pressure\":1021,\"humidity\":51,\"dew_point\":43.56,\"wind_speed\":23.87,\"wind_deg\":188,\"wind_gust\":39.08,\"weather\":[{\"id\":802,\"main\":\"Clouds\",\"description\":\"scattered clouds\",\"icon\":\"03d\"}],\"clouds\":35,\"pop\":0,\"uvi\":6}]}",
    sizeof(json)
  );
  json[sizeof(json) - 1] = '\0';
  }

  printf("Checking DAILY.TEMP.DAY[0]\n");
  faux_parse_json("\"day\":", 0);
  printf("json part = <<<%s>>>\n\n", json_part);

  printf("Checking DAILY.TEMP.DAY[1]\n");
  faux_parse_json("\"day\":", 1);
  printf("json part = <<<%s>>>\n\n", json_part);

  printf("Checking DAILY.TEMP.DAY[2]\n");
  faux_parse_json("\"day\":", 2);
  printf("json part = <<<%s>>>\n\n", json_part);

  return(0); 
}

