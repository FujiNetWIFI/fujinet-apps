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

  return(0); 
}

