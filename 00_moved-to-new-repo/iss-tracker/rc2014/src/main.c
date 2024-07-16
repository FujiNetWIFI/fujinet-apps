/** 
 * iss-tracker for RC2014
 *
 * inspired by Bill Kendrick's Atari version
 *
 * @author justin skists
 * @license gpl v. 3, see LICENSE.md for details
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <z80.h>

#include "console.h"
#include "fujinet.h"
#include "fujinet_network.h"

#include "blue_marb_ascii_72.h"
#include "time_impl.h"

const char *url = "HTTP://api.open-notify.org/iss-now.json";
uint8_t network_unit = 0;

#define NUM_ELEMENTS 3
const char * elements[NUM_ELEMENTS] = {
    "/iss_position/latitude",
    "/iss_position/longitude",
    "/timestamp",
};

#define ELEM_LAT 0
#define ELEM_LNG 1
#define ELEM_TIMESTAMP 2

#define MAP_HEIGHT 18.0
#define MAP_WIDTH 72.0



char query[256];
char buf[256];
struct network_status status;

uint8_t iss_x;
uint8_t iss_y;


char elements_response[NUM_ELEMENTS][32];

void screen_gotoxy(char x, char y) {
    // indexed by zero
    printf("%c[%d;%dH", 27, y+1, x+1);
}

void screen_hide_cursor(bool hide) {
    if (hide) {
        printf("\x1B[?25l");
    } else {
        printf("\x1B[?25h");
    }
}

void clear_map(void) {
    printf("%c[2J", 27);
}

void display_map(void) {
    uint8_t* p = (uint8_t*)blue_marb_ascii_72_txt;

    for (int n = 0; n < blue_marb_ascii_72_txt_len; n++) {
        putchar(*p);
        p++;
    }

    printf("\n\x1B[7mINTERNATIONAL SPACE STATION\x1B[m\n");

    uint32_t timestamp = atol(elements_response[ELEM_TIMESTAMP]);
    struct tm tm;
    secs_to_tm(timestamp,  &tm );

    float lat = atof(elements_response[ELEM_LAT]);
    float lng = atof(elements_response[ELEM_LNG]);


    float xf = (lng + 180.0) * (MAP_WIDTH / 360.0);
    float yf = (lat + 90.0) * (MAP_HEIGHT / 180.0);

    iss_x = truncf(xf);
    iss_y = 18 - truncf(yf);

    printf("\33[2KTime: %04d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("\x1B[2KLatitude:  %s\n", elements_response[ELEM_LAT]);
    printf("\x1B[2KLongitude: %s\n", elements_response[ELEM_LNG]);
}

void display_iss(bool show) {
    screen_gotoxy(iss_x, iss_y);

    if (show) {
        printf("\x1b[97mX\x1b[0m");
    } else {
        printf(" ");
    }
}

FUJINET_RC fetch(void) {
    FUJINET_RC rc = FUJINET_RC_OK;
    uint16_t data_len;

    //printf("Reading elements...\n\n");
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        //printf("Querying \"%s\"...", elements[i]);

        sprintf(query, "%s", elements[i]);
        rc = fujinet_json_query(network_unit, (uint8_t*)query, 256);

        if (rc == FUJINET_RC_OK) {
            //printf("Requesting status...");
            rc = fujinet_network_status(network_unit, &status);

            if (rc == FUJINET_RC_OK) {
                data_len = (status.bytes_waiting_high << 8) + status.bytes_waiting_low;

                if (data_len != 0) {
                    //printf("Reading %d bytes of data...", data_len);

                    memset(buf, 0, sizeof(buf));
                    rc = fujinet_network_read(network_unit, buf, data_len);

                    if (rc == FUJINET_RC_OK) {
                        //printf("FETCHED: %s\n\n", buf);
                        strcpy(elements_response[i], buf);
                    }
                } else {
                    printf("NOT FOUND\n\n");
                }
            }
        }
    }

    return rc;
}

void main(void) {
    FUJINET_RC rc = FUJINET_RC_OK;
    bool done = false;

    fujinet_init();

    screen_hide_cursor(true);
    clear_map();

    while (!done) {
        rc = fujinet_network_open(network_unit, url, 0);
        if (rc == FUJINET_RC_OK) {
            rc = fujinet_channel_mode(network_unit, CHANNELMODE_JSON);
            if (rc == FUJINET_RC_OK) {
                rc = fujinet_json_parse(network_unit);
                if (rc == FUJINET_RC_OK) {
                    fetch();
                }
            }

            fujinet_network_close(network_unit);
        }

        screen_gotoxy(0, 0);
        display_map();

        for (int i = 0; i < 30 && !done; i++) {
            while (console_rx_avail()) {
                uint8_t c = console_rx();
                if (c == 27)
                    done = true;
            };

            if (done)
                break;

            display_iss((bool)(i & 1));
            z80_delay_ms(500);
        }
    }

    clear_map();
    screen_hide_cursor(false);
}
