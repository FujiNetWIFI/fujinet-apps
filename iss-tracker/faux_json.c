#include <stdlib.h>
#include <string.h>
#include "faux_json.h"

#ifdef HOST
#include <stdio.h>
#endif

unsigned char json[1024], json_part[1024];

void faux_parse_json(char * keyword, unsigned char which) {
  int i;

  /* Find the value corresponding to the Nth keyword */
  json_part[0] = '\0';
  for (i = 0; json[i] != '\0' && json_part[0] == '\0'; i++) {
#ifdef HOST
    printf("json[%d] = \"%s\"\n  versus \"%s\" #%d\n  result: %d\n\n",
      i, json + i,
      keyword, which,
      strncmp((unsigned char *) (json + i), keyword, strlen(keyword))
    );
#endif
    if (strncmp((unsigned char *) (json + i), keyword, strlen(keyword)) == 0) {
      if (which != 0) {
        /* Skip until we find the Nth entry */
        which--;
      } else {
        strcpy(json_part, json + i + strlen(keyword));
#ifdef HOST
    printf("json_part = %s\n\n", json_part);
#endif
      }
    }
  }

  if (json_part[0] == '\"') {
    /* It starts with a quote; find the end quote and truncate there */
    for (i = 1; json_part[i] != '\0'; i++) {
      if (json_part[i] == '\"') {
        json_part[i] = '\0';
      }
    }

    /* Now shift it all left, to trim the starting quote */
    for (i = 0; json_part[i] != '\0'; i++) {
      json_part[i] = json_part[i + 1];
    }
  } else {
    /* It did not start with a quote; it must be a number;
       truncate at the first non-number character */
    for (i = 0; json_part[i] != '\0'; i++) {
      if ((json_part[i] < '0' || json_part[i] > '9') &&
          json_part[i] != '-' && json_part[i] != '.') {
        json_part[i] = '\0';
      }
    }
  }
}

